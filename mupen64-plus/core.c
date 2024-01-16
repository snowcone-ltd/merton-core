#include "../core.h"

#include "matoya.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define M64P_CORE_PROTOTYPES
#include "mupen64plus-core/src/api/m64p_frontend.h"
#include "mupen64plus-core/src/main/version.h"

m64p_error RSP_PluginStartup(m64p_dynlib_handle handle, void *opaque,
	void (*debug_callback)(void *opaque, int level, const char *msg));

m64p_error RDP_PluginStartup(m64p_dynlib_handle handle, void *opaque,
	void (*debug_callback)(void *opaque, int level, const char *msg));

void osal_dynlib_set_prefix(const char *prefix);
m64p_dynlib_handle osal_dynlib_get_handle(const char *name);

struct Core {
	bool loaded;
	CoreVideoFunc video_func;
	CoreAudioFunc audio_func;
	void *audio_opaque;
	void *video_opaque;
};

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
	return calloc(1, sizeof(Core));
}

void CoreUnload(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

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
	m64p_error r = CoreStartup(FRONTEND_API_VERSION, ".", ".", NULL, core_debug_callback, 0, core_state_callback);
	if (r != M64ERR_SUCCESS)
		return false;

	size_t size = 0;
	void *game = MTY_ReadFile(path, &size);

	r = CoreDoCommand(M64CMD_ROM_OPEN, size, game);
	MTY_Free(game);

	if (r != M64ERR_SUCCESS)
		return false;

	m64p_dynlib_handle h = osal_dynlib_get_handle("mupen64plus.dll");

	r = RSP_PluginStartup(h, NULL, core_debug_callback);
	core_log("RSP_PluginStartup: %d\n", r);

	r = RDP_PluginStartup(h, NULL, core_debug_callback);
	core_log("RDP_PluginStartup: %d\n", r);

	osal_dynlib_set_prefix("RDP_");
	r = CoreAttachPlugin(M64PLUGIN_GFX, h);
	osal_dynlib_set_prefix("");
	core_log("CoreAttachPlugin (GFX): %d\n", r);

	osal_dynlib_set_prefix("RSP_");
	r = CoreAttachPlugin(M64PLUGIN_RSP, h);
	osal_dynlib_set_prefix("");
	core_log("CoreAttachPlugin (RSP): %d\n", r);

	ctx->loaded = true;

	return true;
}

void CoreUnloadGame(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->loaded = false;
}

bool CoreGameIsLoaded(Core *ctx)
{
	return ctx ? ctx->loaded : false;
}

double CoreGetFrameRate(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 60.0;

	return 60.0;
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 1.0f;

	return 1.0f;
}

void CoreRun(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	core_log("RUN1\n");
	m64p_error r = CoreDoCommand(M64CMD_ADVANCE_FRAME, 0, NULL);
	r = CoreDoCommand(M64CMD_EXECUTE, 0, NULL);
	core_log("RUN2\n");
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	return NULL;
}

void CoreReset(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || !ctx->loaded)
		return;
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	return NULL;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
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
