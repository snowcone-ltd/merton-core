#include "../core.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mgba/include/mgba/core/core.h"
#include "mgba/include/mgba-util/vfs.h"

struct Core {
	struct mCore *core;
	bool loaded;
	CoreVideoFunc video_func;
	CoreAudioFunc audio_func;
	void *audio_opaque;
	void *video_opaque;
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

const char * const projectName = "";
const char * const projectVersion = "";

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

Core *CoreLoad(const char *name, const char *systemDir, const char *saveDir)
{
	Core *ctx = calloc(1, sizeof(Core));

	return ctx;
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
	struct VFile *rom = VFileOpen(path, O_RDONLY);
	if (!rom)
		return false;

	ctx->core = mCoreFindVF(rom);
	if (!ctx->core) {
		rom->close(rom);
		return false;
	}

	mCoreInitConfig(ctx->core, NULL);

	return false;
}

void CoreUnloadGame(Core *ctx)
{
}

bool CoreGameIsLoaded(Core *ctx)
{
	return ctx ? ctx->loaded : false;
}

double CoreGetFrameRate(Core *ctx)
{
	return 60.0;
}

float CoreGetAspectRatio(Core *ctx)
{
	return 1.0f;
}

void CoreRun(Core *ctx)
{
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	return NULL;
}

void CoreReset(Core *ctx)
{
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	return NULL;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
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
	*len = 0;

	return NULL;
}

const char *CoreGetSetting(Core *ctx, const char *key)
{
	return NULL;
}

void CoreSetSetting(Core *ctx, const char *key, const char *val)
{
}

void CoreResetSettings(Core *ctx)
{
}
