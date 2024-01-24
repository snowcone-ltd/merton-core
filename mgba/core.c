#include "../core.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mgba/include/mgba/core/core.h"
#include "mgba/include/mgba/core/blip_buf.h"
#include "mgba/include/mgba/core/serialize.h"
#include "mgba/include/mgba/internal/gba/gba.h"
#include "mgba/include/mgba/internal/gba/memory.h"
#include "mgba/include/mgba/gba/interface.h"
#include "mgba/include/mgba-util/memory.h"
#include "mgba/include/mgba-util/vfs.h"

#define FRAME_WIDTH  256
#define FRAME_HEIGHT 224
#define SAMPLE_RATE  48000
#define SAMPLES_MAX  0x4000

struct Core {
	struct mCore *core;
	bool loaded;
	CoreVideoFunc video_func;
	CoreAudioFunc audio_func;
	void *audio_opaque;
	void *video_opaque;

	color_t frame[FRAME_HEIGHT][FRAME_WIDTH];
	int16_t samples[SAMPLES_MAX];
	uint32_t keys;

	void *flash;
	struct mRumble rumble;
	struct mRotationSource rotation;
	struct GBALuminanceSource lux;
	struct mLogger logger;

	int32_t l;
	int32_t r;

	struct {
		uint8_t lpass;
	} settings;
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

const char * const projectName = "";
const char * const projectVersion = "";

void core_log(const char *fmt, ...)
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

static void core_mgba_log(struct mLogger *logger, int category, enum mLogLevel level, const char *format, va_list args)
{
	if (!CORE_LOG_FUNC)
		return;

	// TODO This generates tons of spam, better filters?
	if (level == mLOG_GAME_ERROR || level == mLOG_INFO || level == mLOG_DEBUG)
		return;

	char msg[512];
	vsnprintf(msg, 512, format, args);

	char wnl[512];
	snprintf(wnl, 512, "%s\n", msg);

	CORE_LOG_FUNC(wnl, CORE_LOG_OPAQUE);
}

static uint8_t core_read_luminance(struct GBALuminanceSource *lux)
{
	// TODO

	return 0;
}

static void core_luminance_sample(struct GBALuminanceSource *lux)
{
	// TODO
}

static void core_set_rumble(struct mRumble *rumble, int enable)
{
	// TODO
}

static void core_rotation_sample(struct mRotationSource *source)
{
	// TODO
}

static int32_t core_read_tilt_x(struct mRotationSource *source)
{
	// TODO

	return 0;
}

static int32_t core_read_tilt_y(struct mRotationSource *source)
{
	// TODO

	return 0;
}

static int32_t core_read_gyro_z(struct mRotationSource *source)
{
	// TODO

	return 0;
}

Core *CoreLoad(const char *systemDir)
{
	Core *ctx = calloc(1, sizeof(Core));

	ctx->rumble.setRumble = core_set_rumble;

	ctx->rotation.sample = core_rotation_sample;
	ctx->rotation.readTiltX = core_read_tilt_x;
	ctx->rotation.readTiltY = core_read_tilt_y;
	ctx->rotation.readGyroZ = core_read_gyro_z;

	ctx->lux.readLuminance = core_read_luminance;
	ctx->lux.sample = core_luminance_sample;

	ctx->settings.lpass = 60;

	ctx->logger.log = core_mgba_log;
	mLogSetDefaultLogger(&ctx->logger);

	return ctx;
}

void CoreUnload(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	CoreUnloadGame(ctx);

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
		rom->close(rom);
		return false;
	}

	memset(ctx->frame, 0xFF, sizeof(ctx->frame));
	ctx->core->setVideoBuffer(ctx->core, (color_t *) ctx->frame, FRAME_WIDTH);
	ctx->core->setAudioBufferSize(ctx->core, SAMPLES_MAX);

	blip_set_rates(ctx->core->getAudioChannel(ctx->core, 0), ctx->core->frequency(ctx->core), SAMPLE_RATE);
	blip_set_rates(ctx->core->getAudioChannel(ctx->core, 1), ctx->core->frequency(ctx->core), SAMPLE_RATE);

	ctx->core->setPeripheral(ctx->core, mPERIPH_RUMBLE, &ctx->rumble);
	ctx->core->setPeripheral(ctx->core, mPERIPH_ROTATION, &ctx->rotation);

	ctx->flash = anonymousMemoryMap(GBA_SIZE_FLASH1M);
	memset(ctx->flash, 0xFF, GBA_SIZE_FLASH1M);

	ctx->core->loadROM(ctx->core, rom);
	ctx->core->setPeripheral(ctx->core, mPERIPH_GBA_LUMINANCE, &ctx->lux);
	ctx->core->reset(ctx->core);

	if (saveData && saveDataSize > 0 && saveDataSize <= GBA_SIZE_FLASH1M)
		memcpy(ctx->flash, saveData, saveDataSize);

	struct VFile *sdf = VFileFromMemory(ctx->flash, GBA_SIZE_FLASH1M);
	if (!ctx->core->loadSave(ctx->core, sdf))
		sdf->close(sdf);

	ctx->loaded = true;

	return true;
}

void CoreUnloadGame(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	mCoreConfigDeinit(&ctx->core->config);
	ctx->core->deinit(ctx->core);

	mappedMemoryFree(ctx->flash, GBA_SIZE_FLASH1M);
	ctx->flash = NULL;

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

	return ctx->core->frequency(ctx->core) / (double) ctx->core->frameCycles(ctx->core);
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 1.0f;

	unsigned width = 0;
	unsigned height = 0;
	ctx->core->baseVideoSize(ctx->core, &width, &height);

	return (float) width / height;
}

static void core_audio_filter(Core *ctx, uint8_t level, int16_t *buf, size_t count)
{
	int32_t a = level * 0x10000 / 100;
	int32_t b = 0x10000 - a;

	for (size_t x = 0, y = 0; x < count; x++, y += 2) {
		ctx->l = (ctx->l * a + buf[y] * b) >> 16;
		ctx->r = (ctx->r * a + buf[y + 1] * b) >> 16;

		buf[y] = (int16_t) ctx->l;
		buf[y + 1] = (int16_t) ctx->r;
	}
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
			width, height, FRAME_WIDTH * sizeof(color_t), ctx->video_opaque);

	if (ctx->audio_func) {
		blip_t *l = ctx->core->getAudioChannel(ctx->core, 0);
		blip_t *r = ctx->core->getAudioChannel(ctx->core, 1);

		int available = blip_samples_avail(l);

		if (available > 0) {
			int frames = blip_read_samples(l, ctx->samples, SAMPLES_MAX, true);
			blip_read_samples(r, ctx->samples + 1, SAMPLES_MAX, true);

			if (ctx->settings.lpass > 0)
				core_audio_filter(ctx, ctx->settings.lpass, ctx->samples, frames);

			ctx->audio_func(ctx->samples, frames, SAMPLE_RATE, ctx->audio_opaque);
		}
	}
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	if (ctx->core->platform(ctx->core) != mPLATFORM_GBA)
		return NULL;

	struct GBA *gba = (struct GBA *) ctx->core->board;

	if (gba->memory.savedata.type == SAVEDATA_AUTODETECT) {
		*size = GBA_SIZE_FLASH1M;

	} else {
		*size = GBASavedataSize(&gba->memory.savedata);
	}

	void *sd = malloc(*size);
	memcpy(sd, ctx->flash, *size);

	return sd;
}

void CoreReset(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->core->reset(ctx->core);
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || !ctx->loaded)
		return;

	uint32_t k = 0;

	switch (button) {
		case CORE_BUTTON_A:      k = 0x001; break;
		case CORE_BUTTON_B:      k = 0x002; break;
		case CORE_BUTTON_SELECT: k = 0x004; break;
		case CORE_BUTTON_START:  k = 0x008; break;
		case CORE_BUTTON_DPAD_R: k = 0x010; break;
		case CORE_BUTTON_DPAD_L: k = 0x020; break;
		case CORE_BUTTON_DPAD_U: k = 0x040; break;
		case CORE_BUTTON_DPAD_D: k = 0x080; break;
		case CORE_BUTTON_R:      k = 0x100; break;
		case CORE_BUTTON_L:      k = 0x200; break;
	}

	if (pressed) {
		ctx->keys |= k;

	} else {
		ctx->keys &= ~k;
	}

	ctx->core->setKeys(ctx->core, ctx->keys);
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	struct VFile *vfm = VFileMemChunk(NULL, 0);
	mCoreSaveStateNamed(ctx->core, vfm, SAVESTATE_SAVEDATA | SAVESTATE_RTC);

	*size = vfm->size(vfm);

	void *state = malloc(*size);

	vfm->seek(vfm, 0, SEEK_SET);
	vfm->read(vfm, state, *size);
	vfm->close(vfm);

	return state;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx || !ctx->loaded)
		return false;

	struct VFile *vfm = VFileFromConstMemory(state, size);

	bool success = mCoreLoadStateNamed(ctx->core, vfm, SAVESTATE_RTC);
	vfm->close(vfm);

	return success;
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
