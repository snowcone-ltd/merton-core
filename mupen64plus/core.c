#include "../core.h"

#include "matoya.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define M64P_CORE_PROTOTYPES
#include "mupen64plus-core/src/api/m64p_frontend.h"
#include "mupen64plus-core/src/main/version.h"


// Plugins

m64p_error RSP_PluginStartup(m64p_dynlib_handle handle, void *opaque,
	void (*debug_callback)(void *opaque, int level, const char *msg));
m64p_error RSP_PluginShutdown(void);

m64p_error RDP_PluginStartup(m64p_dynlib_handle handle, void *opaque,
	void (*debug_callback)(void *opaque, int level, const char *msg));
m64p_error RDP_PluginShutdown(void);
void RDP_ReadScreen2(void *dest, int *width, int *height, int front);


// Shim

void osal_dynlib_set_prefix(const char *prefix);
m64p_dynlib_handle osal_dynlib_get_handle(const char *name);


// Core

struct Core {
	bool loaded;
	MTY_Thread *game_thread;
	CoreVideoFunc video_func;
	CoreAudioFunc audio_func;
	void *audio_opaque;
	void *video_opaque;
};

static MTY_Mutex *CORE_MUTEX;
static MTY_Cond *CORE_COND;

static uint32_t CORE_FRAME[1024][1024];
static int32_t CORE_WIDTH;
static int32_t CORE_HEIGHT;

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

static void core_log(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	if (CORE_LOG_FUNC) {
		char msg[1024];
		vsnprintf(msg, 1024, fmt, arg);

		CORE_LOG_FUNC(msg, CORE_LOG_OPAQUE);
	}

	va_end(arg);
}

static void core_debug_callback(void *aContext, int aLevel, const char *aMessage)
{
	core_log("%s\n", aMessage);
}

static void core_state_callback(void *Context, m64p_core_param param_type, int new_value)
{
}

Core *CoreLoad(const char *systemDir, const char *saveDir)
{
	Core *ctx = calloc(1, sizeof(Core));

	CORE_MUTEX = MTY_MutexCreate();
	CORE_COND = MTY_CondCreate();

	return ctx;
}

void CoreUnload(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	CoreUnloadGame(ctx);

	MTY_CondDestroy(&CORE_COND);
	MTY_MutexDestroy(&CORE_MUTEX);

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
	ctx->audio_func = func;
	ctx->audio_opaque = opaque;
}

void CoreSetVideoFunc(Core *ctx, CoreVideoFunc func, void *opaque)
{
	ctx->video_func = func;
	ctx->video_opaque = opaque;
}

bool CoreLoadGame(Core *ctx, CoreSystem system, const char *path, const void *saveData,
	size_t saveDataSize)
{
	m64p_error r = CoreStartup(FRONTEND_API_VERSION, ".", ".", NULL, core_debug_callback,
		NULL, core_state_callback);

	if (r != M64ERR_SUCCESS)
		return false;

	size_t size = 0;
	void *game = MTY_ReadFile(path, &size);

	r = CoreDoCommand(M64CMD_ROM_OPEN, size, game);
	MTY_Free(game);

	if (r != M64ERR_SUCCESS)
		return false;

	m64p_dynlib_handle h = osal_dynlib_get_handle("mupen64plus.dll");

	RDP_PluginStartup(h, NULL, core_debug_callback);
	RSP_PluginStartup(h, NULL, core_debug_callback);

	osal_dynlib_set_prefix("RDP_");
	CoreAttachPlugin(M64PLUGIN_GFX, h);

	osal_dynlib_set_prefix("RSP_");
	CoreAttachPlugin(M64PLUGIN_RSP, h);

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
		MTY_ThreadDestroy(&ctx->game_thread);

	CoreDetachPlugin(M64PLUGIN_RSP);
	CoreDetachPlugin(M64PLUGIN_GFX);

	RSP_PluginShutdown();
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
	MTY_MutexLock(CORE_MUTEX);

	RDP_ReadScreen2(CORE_FRAME, &CORE_WIDTH, &CORE_HEIGHT, 0);
	MTY_CondSignal(CORE_COND);

	MTY_MutexUnlock(CORE_MUTEX);
}

static void *core_game_thread(void *opaque)
{
	Core *ctx = opaque;

	core_log("Game thread started\n");

	CoreDoCommand(M64CMD_SET_FRAME_CALLBACK, 0, core_frame_callback);
	m64p_error r = CoreDoCommand(M64CMD_EXECUTE, 0, NULL);

	core_log("Game thread ended: %d\n", r);

	return NULL;
}

void CoreRun(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	if (!ctx->game_thread)
		ctx->game_thread = MTY_ThreadCreate(core_game_thread, ctx);

	MTY_MutexLock(CORE_MUTEX);

	MTY_CondWait(CORE_COND, CORE_MUTEX, 200);

	if (ctx->video_func && CORE_WIDTH > 0 && CORE_HEIGHT > 0)
		ctx->video_func(CORE_FRAME, CORE_COLOR_FORMAT_RGBA,
			CORE_WIDTH, CORE_HEIGHT, CORE_WIDTH * 4, ctx->video_opaque);

	MTY_MutexUnlock(CORE_MUTEX);
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
	// TODO

	if (!ctx || !ctx->loaded)
		return NULL;

	return NULL;
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	// TODO

	if (!ctx || !ctx->loaded)
		return;
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
	// TODO
}

void *CoreGetState(Core *ctx, size_t *size)
{
	// TODO

	if (!ctx || !ctx->loaded)
		return NULL;

	return NULL;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	// TODO

	if (!ctx || !ctx->loaded)
		return false;

	return false;
}

uint8_t CoreGetNumDisks(Core *ctx)
{
	return 0;
}

int8_t CoreGetDisk(Core *ctx)
{
	return -1;
}

bool CoreSetDisk(Core *ctx, int8_t disk, const char *path)
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
