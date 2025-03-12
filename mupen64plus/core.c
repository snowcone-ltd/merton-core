#include "../core.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if defined(_WIN32)
	#include <windows.h>
	#define USE_PRSP false
	#define USE_PRDP true
#else
	#include <dlfcn.h>
	#define USE_PRSP true

	#if defined(__APPLE__) || defined(__aarch64__)
		#define USE_PRDP false
	#else
		#define USE_PRDP true
	#endif
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

void audio_set_func(CoreAudioFunc func, void *opaque);

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

void vdac_set_func(void (*func)(void *, uint32_t, uint32_t, void *), void *opaque);


// Core

struct Core {
	bool use_prdp;
	bool use_prsp;
	m64p_dynlib_handle so;

	uint32_t frame[5120][1920];
	uint32_t w;
	uint32_t h;

	uint64_t frame_ctr;
	uint64_t prev_frame_ctr;
	SDL_Thread *game_thread;
	SDL_Mutex *mutex;
	SDL_Condition *cond;
};

static CoreLogFunc CORE_LOG;
static CoreVideoFunc CORE_VIDEO;
static void *CORE_LOG_OPAQUE;
static void *CORE_VIDEO_OPAQUE;

void core_vlog(const char *fmt, va_list arg)
{
	char msg[1024];
	vsnprintf(msg, 1024, fmt, arg);

	CORE_LOG(msg, CORE_LOG_OPAQUE);
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

void CoreUnloadGame(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	CoreDoCommand(M64CMD_STOP, 0, NULL);

	if (ctx->game_thread)
		SDL_WaitThread(ctx->game_thread, NULL);

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

	vdac_set_func(NULL, NULL);
	prdp_set_func(NULL, NULL);
	audio_set_func(NULL, NULL);

	if (ctx->cond)
		SDL_DestroyCondition(ctx->cond);

	if (ctx->mutex)
		SDL_DestroyMutex(ctx->mutex);

	osal_shutdown();
	osal_dynlib_close_handle(ctx->so);

	free(ctx);
	*core = NULL;
}

void CoreSetLogFunc(CoreLogFunc func, void *opaque)
{
	CORE_LOG = func;
	CORE_LOG_OPAQUE = opaque;
}

void CoreSetAudioFunc(CoreAudioFunc func, void *opaque)
{
	audio_set_func(func, opaque);
}

void CoreSetVideoFunc(CoreVideoFunc func, void *opaque)
{
	CORE_VIDEO = func;
	CORE_VIDEO_OPAQUE = opaque;
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

Core *CoreLoadGame(CoreSystem system, const char *systemDir, const char *path,
	const void *saveData, size_t saveDataSize)
{
	bool loaded = true;

	Core *ctx = calloc(1, sizeof(Core));

	// TODO These need to be settings
	ctx->use_prdp = USE_PRDP;
	ctx->use_prsp = USE_PRSP;

	const char *so_name = "mupen64plus";

	#if !defined(_WIN32)
		Dl_info info = {0};

		if (dladdr(CoreLoadGame, &info) && info.dli_fname)
			so_name = info.dli_fname;
	#endif

	ctx->so = osal_dynlib_get_handle(so_name);
	osal_startup();

	ctx->mutex = SDL_CreateMutex();
	ctx->cond = SDL_CreateCondition();

	m64p_error r = CoreStartup(FRONTEND_API_VERSION, systemDir, systemDir,
		NULL, core_debug_callback, NULL, NULL);

	if (r != M64ERR_SUCCESS) {
		loaded = false;
		goto except;
	}

	size_t size = 0;
	void *game = core_load_file(path, &size);

	r = CoreDoCommand(M64CMD_ROM_OPEN, size, game);
	free(game);

	if (r != M64ERR_SUCCESS) {
		loaded = false;
		goto except;
	}

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

	except:

	if (!loaded)
		CoreUnloadGame(&ctx);

	return ctx;
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
	vdac_set_func(core_new_frame, opaque);
	prdp_set_func(core_new_frame, opaque);

	CoreDoCommand(M64CMD_EXECUTE, 0, NULL);

	return 0;
}

void CoreRun(Core *ctx)
{
	if (!ctx)
		return;

	CorePauseThreads(ctx, false);

	if (!ctx->game_thread)
		ctx->game_thread = SDL_CreateThread(core_game_thread, NULL, ctx);

	SDL_LockMutex(ctx->mutex);

	if (ctx->prev_frame_ctr == ctx->frame_ctr)
		SDL_WaitCondition(ctx->cond, ctx->mutex);

	ctx->prev_frame_ctr = ctx->frame_ctr;

	if (ctx->w > 0 && ctx->h > 0) {
		CORE_VIDEO(ctx->frame, CORE_COLOR_FORMAT_RGBA, ctx->w, ctx->h,
			ctx->w * 4, CORE_VIDEO_OPAQUE);

	} else {
		CORE_VIDEO(NULL, CORE_COLOR_FORMAT_UNKNOWN, 0, 0, 0, CORE_VIDEO_OPAQUE);
	}

	SDL_UnlockMutex(ctx->mutex);
}

void CorePauseThreads(Core *ctx, bool pause)
{
	if (!ctx)
		return;

	g_rom_pause = pause ? 1 : 0;
}

void CoreReset(Core *ctx)
{
	if (!ctx)
		return;

	// '1' means 'hard reset'
	CoreDoCommand(M64CMD_RESET, 1, NULL);
}

double CoreGetFrameRate(Core *ctx)
{
	return 60.0;
}

float CoreGetAspectRatio(Core *ctx)
{
	return 4.0f / 3.0f;
}

void *CoreGetSaveData(Core *ctx, size_t *size, CoreSaveDataType *type)
{
	if (!ctx)
		return NULL;

	void *sd = NULL;
	void *ptr = NULL;

	struct file_storage *eeprom = g_dev.cart.eeprom.storage;
	struct file_storage *sram = g_dev.cart.sram.storage;
	struct file_storage *flashram = g_dev.cart.flashram.storage;

	switch (ROM_SETTINGS.savetype) {
		case SAVETYPE_EEPROM_4K:
			*size = 0x200;
			*type = CORE_SAVE_DATA_EEPROM;
			ptr = eeprom->data;
			break;
		case SAVETYPE_EEPROM_16K:
			*size = 0x800;
			*type = CORE_SAVE_DATA_EEPROM;
			ptr = eeprom->data;
			break;
		case SAVETYPE_SRAM:
			*size = SRAM_SIZE;
			*type = CORE_SAVE_DATA_SRAM;
			ptr = sram->data;
			break;
		case SAVETYPE_FLASH_RAM:
			*size = FLASHRAM_SIZE;
			*type = CORE_SAVE_DATA_FLASH_RAM;
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
	if (!ctx)
		return;

	input_set_button(player, button, pressed);
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
	if (!ctx)
		return;

	input_set_axis(player, axis, value);
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx)
		return NULL;

	void *state = NULL;
	*size = 0;

	osal_lock();

	CorePauseThreads(ctx, false);
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
	if (!ctx)
		return false;

	osal_set_read_data(state, size);

	CorePauseThreads(ctx, false);
	m64p_error r = CoreDoCommand(M64CMD_STATE_LOAD, 0, "memory");

	return r == M64ERR_SUCCESS;
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	return false;
}

CoreSetting *CoreGetSettings(uint32_t *len)
{
	// TODO

	*len = 0;

	return NULL;
}

void CoreUpdateSettings(Core *ctx)
{
	// TODO

	if (!ctx)
		return;
}
