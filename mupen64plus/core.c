#include "../core.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if defined(_WIN32)
	#include <windows.h>
	#define USE_PRSP false
#else
	#include <dlfcn.h>
	#define _strdup strdup
	#define USE_PRSP true
#endif

#define M64P_CORE_PROTOTYPES
#include "mupen64plus-core/src/api/m64p_frontend.h"
#include "mupen64plus-core/src/backends/file_storage.h"
#include "mupen64plus-core/src/main/main.h"
#include "mupen64plus-core/src/main/rom.h"
#include "mupen64plus-core/src/main/savestates.h"
#include "mupen64plus-core/src/main/version.h"

#include "shim/parallel-rdp/prdp.h"

#include "SDL.h" // Not the real SDL


// Plugins

m64p_error RDP_PluginStartup(m64p_dynlib_handle handle, void *opaque,
	void (*debug_callback)(void *opaque, int level, const char *msg));
m64p_error RDP_PluginShutdown(void);

void audio_set_callback(CoreAudioFunc func, void *opaque);

void input_set_button(uint8_t player, CoreButton button, bool pressed);
void input_set_axis(uint8_t player, CoreAxis axis, int16_t value);

m64p_error RSP_PluginStartup(m64p_dynlib_handle handle, void *opaque,
	void (*debug_callback)(void *opaque, int level, const char *msg));
m64p_error RSP_PluginShutdown(void);


// Shim

void osal_startup(void);
void osal_shutdown(void);

void osal_lock(void);
void osal_unlock(void);
bool osal_wait(int32_t timeout);

const void *osal_get_write_buf(size_t *size);
void osal_set_read_data(const void *buf, size_t size);

void osal_dynlib_set_prefix(const char *prefix);
m64p_dynlib_handle osal_dynlib_get_handle(const char *name);
void osal_dynlib_close_handle(m64p_dynlib_handle h);

void vdac_set_video_func(void (*func)(void *, uint32_t, uint32_t, void *), void *opaque);


// Core

struct Core {
	bool loaded;
	bool use_prdp;
	bool use_prsp;
	char *system_dir;
	m64p_dynlib_handle so;

	uint32_t frame[5120][1920];
	uint32_t w;
	uint32_t h;

	uint64_t frame_ctr;
	uint64_t prev_frame_ctr;
	SDL_Thread *game_thread;
	SDL_Mutex *mutex;
	SDL_Condition *cond;

	CoreVideoFunc video_func;
	void *video_opaque;
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

void core_vlog(const char *fmt, va_list arg)
{
	if (CORE_LOG_FUNC) {
		char msg[1024];
		vsnprintf(msg, 1024, fmt, arg);

		CORE_LOG_FUNC(msg, CORE_LOG_OPAQUE);
	}
}

void core_log(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	core_vlog(fmt, arg);
	va_end(arg);
}

static void core_debug_callback(void *aContext, int aLevel, const char *aMessage)
{
	// Filter messages about empty config
	if (strstr(aMessage, "No version number"))
		return;

	core_log("%s\n", aMessage);
}

static void core_state_callback(void *Context, m64p_core_param param_type, int new_value)
{
}

Core *CoreLoad(const char *systemDir)
{
	Core *ctx = calloc(1, sizeof(Core));

	ctx->system_dir = _strdup(systemDir);

	const char *so_name = "mupen64plus";

	#if !defined(_WIN32)
		Dl_info info = {0};

		if (dladdr(CoreLoad, &info) && info.dli_fname)
			so_name = info.dli_fname;
	#endif

	ctx->so = osal_dynlib_get_handle(so_name);

	osal_startup();

	ctx->mutex = SDL_CreateMutex();
	ctx->cond = SDL_CreateCondition();

	return ctx;
}

void CoreUnload(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	CoreUnloadGame(ctx);

	SDL_DestroyCondition(ctx->cond);
	SDL_DestroyMutex(ctx->mutex);

	audio_set_callback(NULL, NULL);

	osal_shutdown();

	osal_dynlib_close_handle(ctx->so);
	free(ctx->system_dir);

	free(ctx);
	*core = NULL;
}

void CoreSetLogFunc(Core *ctx, CoreLogFunc func, void *opaque)
{
	CORE_LOG_FUNC = func;
	CORE_LOG_OPAQUE = opaque;
}

void CoreSetAudioFunc(Core *ctx, CoreAudioFunc func, void *opaque)
{
	audio_set_callback(func, opaque);
}

void CoreSetVideoFunc(Core *ctx, CoreVideoFunc func, void *opaque)
{
	ctx->video_func = func;
	ctx->video_opaque = opaque;
}

static void *core_load_file(const char *path, size_t *size)
{
	FILE *f = fopen(path, "rb");
	if (!f)
		return NULL;

	long fsize = 0;
	void *data = NULL;

	if (fseek(f, 0, SEEK_END) == 0) {
		fsize = ftell(f);

		if (fsize != -1) {
			rewind(f);

			*size = fsize;
			data = malloc(fsize);

			fread(data, fsize, 1, f);
		}
	}

	fclose(f);

	return data;
}

bool CoreLoadGame(Core *ctx, CoreSystem system, const char *path, const void *saveData,
	size_t saveDataSize)
{
	// TODO These need to be settings
	ctx->use_prdp = true;
	ctx->use_prsp = USE_PRSP;

	m64p_error r = CoreStartup(FRONTEND_API_VERSION, ctx->system_dir, ctx->system_dir,
		NULL, core_debug_callback, NULL, core_state_callback);

	if (r != M64ERR_SUCCESS)
		return false;

	size_t size = 0;
	void *game = core_load_file(path, &size);

	r = CoreDoCommand(M64CMD_ROM_OPEN, size, game);
	free(game);

	if (r != M64ERR_SUCCESS)
		return false;

	if (!ctx->use_prdp)
		RDP_PluginStartup(ctx->so, NULL, core_debug_callback);

	if (!ctx->use_prsp)
		RSP_PluginStartup(ctx->so, NULL, core_debug_callback);

	osal_dynlib_set_prefix(ctx->use_prdp ? "PRDP_" : "RDP_");
	CoreAttachPlugin(M64PLUGIN_GFX, ctx->so);

	osal_dynlib_set_prefix("AUDIO_");
	CoreAttachPlugin(M64PLUGIN_AUDIO, ctx->so);

	osal_dynlib_set_prefix("INPUT_");
	CoreAttachPlugin(M64PLUGIN_INPUT, ctx->so);

	osal_dynlib_set_prefix(ctx->use_prsp ? "PRSP_" : "RSP_");
	CoreAttachPlugin(M64PLUGIN_RSP, ctx->so);

	if (saveData)
		osal_set_read_data(saveData, saveDataSize);

	// int param = 0;
	// CoreDoCommand(M64CMD_CORE_STATE_SET, M64CORE_SPEED_LIMITER, &param);

	ctx->loaded = true;

	return true;
}

void CoreUnloadGame(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->loaded = false;

	CoreDoCommand(M64CMD_STOP, 0, NULL);

	if (ctx->game_thread)
		SDL_WaitThread(ctx->game_thread, NULL);

	vdac_set_video_func(NULL, NULL);
	prdp_set_video_func(NULL, NULL);

	CoreDetachPlugin(M64PLUGIN_RSP);
	CoreDetachPlugin(M64PLUGIN_INPUT);
	CoreDetachPlugin(M64PLUGIN_AUDIO);
	CoreDetachPlugin(M64PLUGIN_GFX);

	if (!ctx->use_prsp)
		RSP_PluginShutdown();

	if (!ctx->use_prdp)
		RDP_PluginShutdown();

	CoreDoCommand(M64CMD_ROM_CLOSE, 0, NULL);
	CoreShutdown();
}

bool CoreGameIsLoaded(Core *ctx)
{
	return ctx ? ctx->loaded : false;
}

static void core_frame_callback(unsigned int index)
{
}

static void core_new_frame(void *pixels, uint32_t width, uint32_t height, void *opaque)
{
	Core *ctx = opaque;

	SDL_LockMutex(ctx->mutex);

	ctx->w = width;
	ctx->h = height;
	memcpy(ctx->frame, pixels, width * height * 4);

	ctx->frame_ctr++;
	SDL_SignalCondition(ctx->cond);

	SDL_UnlockMutex(ctx->mutex);
}

static int core_game_thread(void *opaque)
{
	vdac_set_video_func(core_new_frame, opaque);
	prdp_set_video_func(core_new_frame, opaque);

	CoreDoCommand(M64CMD_EXECUTE, 0, NULL);

	return 0;
}

void CoreRun(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	if (!ctx->game_thread)
		ctx->game_thread = SDL_CreateThread(core_game_thread, NULL, ctx);

	SDL_LockMutex(ctx->mutex);

	// g_rom_pause = 0;

	if (ctx->prev_frame_ctr == ctx->frame_ctr)
		SDL_WaitCondition(ctx->cond, ctx->mutex);

	ctx->prev_frame_ctr = ctx->frame_ctr;

	if (ctx->video_func && ctx->w > 0 && ctx->h > 0) {
		ctx->video_func(ctx->frame, CORE_COLOR_FORMAT_RGBA, ctx->w, ctx->h,
			ctx->w * 4, ctx->video_opaque);

	} else if (ctx->video_func) {
		uint32_t dummy[16][16] = {0};
		ctx->video_func(dummy, CORE_COLOR_FORMAT_RGBA, 16, 16, 16 * 4, ctx->video_opaque);
	}

	// g_rom_pause = 1;

	SDL_UnlockMutex(ctx->mutex);
}

void CoreReset(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	CoreDoCommand(M64CMD_RESET, 0, NULL);
}

double CoreGetFrameRate(Core *ctx)
{
	return 60.0;
}

float CoreGetAspectRatio(Core *ctx)
{
	return 4.0f / 3.0f;
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	void *sd = NULL;
	void *ptr = NULL;

	struct file_storage *eeprom = g_dev.cart.eeprom.storage;
	struct file_storage *sram = g_dev.cart.sram.storage;
	struct file_storage *flashram = g_dev.cart.flashram.storage;

	switch (ROM_SETTINGS.savetype) {
		case SAVETYPE_EEPROM_4K:
		case SAVETYPE_EEPROM_16K:
			*size = 0x800;
			ptr = eeprom->data;
			break;
		case SAVETYPE_SRAM:
			*size = SRAM_SIZE;
			ptr = sram->data;
			break;
		case SAVETYPE_FLASH_RAM:
			*size = FLASHRAM_SIZE;
			ptr = flashram->data;
			break;
	}

	if (ptr) {
		sd = malloc(*size);
		memcpy(sd, ptr, *size);
	}

	return sd;
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || !ctx->loaded)
		return;

	input_set_button(player, button, pressed);
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
	if (!ctx || !ctx->loaded)
		return;

	input_set_axis(player, axis, value);
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	void *state = NULL;
	*size = 0;

	osal_lock();

	m64p_error r = CoreDoCommand(M64CMD_STATE_SAVE, savestates_type_m64p, "memory");

	if (r == M64ERR_SUCCESS) {
		if (osal_wait(1000)) {
			const void *buf = osal_get_write_buf(size);

			state = malloc(*size);
			memcpy(state, buf, *size);
		}
	}

	osal_unlock();

	return state;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx || !ctx->loaded)
		return false;

	osal_set_read_data(state, size);

	m64p_error r = CoreDoCommand(M64CMD_STATE_LOAD, 0, "memory");

	return r == M64ERR_SUCCESS;
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	return false;
}

const CoreSetting *CoreGetAllSettings(Core *ctx, uint32_t *len)
{
	// TODO

	*len = 0;

	return NULL;
}

const char *CoreGetSetting(Core *ctx, const char *key)
{
	// TODO

	return NULL;
}

void CoreSetSetting(Core *ctx, const char *key, const char *val)
{
	// TODO
}

void CoreResetSettings(Core *ctx)
{
	// TODO
}
