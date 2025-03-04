#include "../core.h"

#include <stdlib.h>

struct Core {
	bool dummy;
};

void CoreUnloadGame(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	free(ctx);
	*core = NULL;
}

void CoreSetLogFunc(CoreLogFunc func, void *opaque)
{
}

void CoreSetAudioFunc(CoreAudioFunc func, void *opaque)
{
}

void CoreSetVideoFunc(CoreVideoFunc func, void *opaque)
{
}

Core *CoreLoadGame(CoreSystem system, const char *systemDir, const char *path,
	const void *saveData, size_t saveDataSize)
{
	Core *ctx = (Core *) calloc(1, sizeof(Core));

	return ctx;
}

double CoreGetFrameRate(Core *ctx)
{
	return 60;
}

float CoreGetAspectRatio(Core *ctx)
{
	return 4.0f / 3.0f;
}

void CoreRun(Core *ctx)
{
	if (!ctx)
		return;
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	if (!ctx)
		return NULL;

	return NULL;
}

void CoreReset(Core *ctx)
{
	if (!ctx)
		return;
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx)
		return;
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx)
		return NULL;

	return NULL;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx)
		return false;

	return false;
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	if (!ctx)
		return false;

	return false;
}

CoreSetting *CoreGetSettings(uint32_t *len)
{
	*len = 0;

	return NULL;
}

void CoreUpdateSettings(Core *ctx)
{
	if (!ctx)
		return;
}
