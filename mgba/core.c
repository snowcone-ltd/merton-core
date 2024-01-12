#include "../core.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mgba/include/mgba/core/core.h"
#include "mgba/include/mgba/core/blip_buf.h"
#include "mgba/include/mgba/internal/gba/memory.h"
#include "mgba/include/mgba/gba/interface.h"
#include "mgba/include/mgba-util/memory.h"
#include "mgba/include/mgba-util/vfs.h"

#define SAMPLE_RATE 32768

struct Core {
	struct mCore *core;
	bool loaded;
	CoreVideoFunc video_func;
	CoreAudioFunc audio_func;
	void *audio_opaque;
	void *video_opaque;

	color_t frame[224][256];
	struct mRumble rumble;
	struct mRotationSource rotation;
	struct GBALuminanceSource lux;
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

const char * const projectName = "";
const char * const projectVersion = "";

int read(int const fd, void * const buffer, unsigned const buffer_size)
{
	return _read(fd, buffer, buffer_size);
}

int write(int fd, const void *buffer, unsigned int count)
{
	return _write(fd, buffer, count);
}

int close(int fd)
{
	return _close(fd);
}

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

static uint8_t core_read_luminance(struct GBALuminanceSource *lux)
{
	return 0;
}

static void core_luminance_sample(struct GBALuminanceSource *lux)
{
}

static void core_set_rumble(struct mRumble *rumble, int enable)
{
}

static void core_rotation_sample(struct mRotationSource *source)
{
}

static int32_t core_read_tilt_x(struct mRotationSource *source)
{
	return 0;
}

static int32_t core_read_tilt_y(struct mRotationSource *source)
{
	return 0;
}

static int32_t core_read_gyro_z(struct mRotationSource *source)
{
	return 0;
}

Core *CoreLoad(const char *systemDir, const char *saveDir)
{
	Core *ctx = calloc(1, sizeof(Core));

	ctx->rumble.setRumble = core_set_rumble;

	ctx->rotation.sample = core_rotation_sample;
	ctx->rotation.readTiltX = core_read_tilt_x;
	ctx->rotation.readTiltY = core_read_tilt_y;
	ctx->rotation.readGyroZ = core_read_gyro_z;

	ctx->lux.readLuminance = core_read_luminance;
	ctx->lux.sample = core_luminance_sample;

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
	ctx->core->init(ctx->core);

	if (ctx->core->platform(ctx->core) != mPLATFORM_GBA) {
		// XXX gogo free
		return false;
	}

	memset(ctx->frame, 0xFF, sizeof(ctx->frame));
	ctx->core->setVideoBuffer(ctx->core, (color_t *) ctx->frame, 256);
	ctx->core->setAudioBufferSize(ctx->core, 0x4000);

	blip_set_rates(ctx->core->getAudioChannel(ctx->core, 0), ctx->core->frequency(ctx->core), SAMPLE_RATE);
	blip_set_rates(ctx->core->getAudioChannel(ctx->core, 1), ctx->core->frequency(ctx->core), SAMPLE_RATE);

	ctx->core->setPeripheral(ctx->core, mPERIPH_RUMBLE, &ctx->rumble);
	ctx->core->setPeripheral(ctx->core, mPERIPH_ROTATION, &ctx->rotation);

	void *flash = anonymousMemoryMap(GBA_SIZE_FLASH1M);
	memset(flash, 0xFF, GBA_SIZE_FLASH1M);

	ctx->core->loadROM(ctx->core, rom);
	ctx->core->setPeripheral(ctx->core, mPERIPH_GBA_LUMINANCE, &ctx->lux);
	ctx->core->reset(ctx->core);

	ctx->loaded = true;

	return true;
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
	if (!ctx || !ctx->loaded)
		return;

	ctx->core->runFrame(ctx->core);

	unsigned width = 0;
	unsigned height = 0;
	ctx->core->currentVideoSize(ctx->core, &width, &height);

	if (ctx->video_func)
		ctx->video_func(ctx->frame, CORE_COLOR_FORMAT_B5G6R5,
			width, height, width * 2, ctx->video_opaque);
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
