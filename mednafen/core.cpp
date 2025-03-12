#include "mednafen.h"
#include "state-driver.h"
#include "netplay-driver.h"
#include "MemoryStream.h"
#include "cdrom/CDInterface.h"
#include "psx/psx.h"
#include "psx/cdc.h"
#include "pce/pce.h"
#include "pce/huc.h"

#include "../core.h"

#include "shim/filestream-cb.h"

#include <stdlib.h>

#define CORE_SAMPLE_RATE   44100
#define CORE_AUDIO_BUF_MAX (5 * 1024 * 1024)

using namespace Mednafen;

struct Core {
	MDFNGI *gi;
	MDFN_Surface *surface;
	CDInterface *cdif;
	uint8_t *cropped;
	uint8_t *input;
	int16_t *audio;
	int32_t *line_widths;

	CoreSystem system;
	void *sdata;
	bool sdata_dirty;
	size_t sdata_size_max;
	size_t sdata_size_cur;
};

static CoreLogFunc CORE_LOG;
static void *CORE_LOG_OPAQUE;

static CoreVideoFunc CORE_VIDEO;
static void *CORE_VIDEO_OPAQUE;

static CoreAudioFunc CORE_AUDIO;
static void *CORE_AUDIO_OPAQUE;

static uint8_t *CORE_SS_RESET;


// Logging

void core_log(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char msg[1024];
	vsnprintf(msg, 1024, fmt, arg);

	if (CORE_LOG)
		CORE_LOG(msg, CORE_LOG_OPAQUE);

	va_end(arg);
}


// Mednafen driver callbacks

void Mednafen::MDFND_OutputNotice(MDFN_NoticeType t, const char* s) noexcept
{
	core_log("%s\n", s);
}

void Mednafen::MDFND_OutputInfo(const char *s) noexcept
{
	core_log("%s", s);
}

void Mednafen::MDFND_SetStateStatus(StateStatusStruct *status) noexcept
{
}

void Mednafen::MDFND_NetplayText(const char* text, bool NetEcho)
{
}

void Mednafen::MDFND_MediaSetNotification(uint32 drive_idx, uint32 state_idx, uint32 media_idx, uint32 orientation_idx)
{
	core_log("Media set: drive_idx=%u, state_idx=%u, media_idx=%u\n", drive_idx, state_idx, media_idx);
}

void Mednafen::MDFND_MidSync(EmulateSpecStruct *espec, const unsigned flags)
{
}

void Mednafen::MDFND_SetMovieStatus(StateStatusStruct *status) noexcept
{
}


// Save data conversion (mister compatibility)

static const uint8_t CORE_SS_MAGIC_PACKED[12] = {0xFF, 'B', 0xFF, 'a', 0xFF, 'c', 0xFF, 'k', 0xFF, 'U', 0xFF, 'p'};
static const uint8_t CORE_SS_MAGIC_UNPACKED[6] = {'B', 'a', 'c', 'k', 'U', 'p'};

static void *core_unpack_ss_sdata(const void *psdata, size_t sdata_size, size_t *size)
{
	if (memcmp(psdata, CORE_SS_MAGIC_PACKED, 12) == 0 && sdata_size >= 64 * 1024) {
		*size = 32 * 1024;
		uint8_t *sdata = (uint8_t *) malloc(*size);

		for (size_t x = 1, y = 0; x < 64 * 1024; x += 2, y++)
			sdata[y] = ((const uint8_t *) psdata)[x];

		return sdata;
	}

	return NULL;
}

static void *core_pack_ss_sdata(const void *sdata, size_t sdata_size, size_t *size)
{
	if (memcmp(sdata, CORE_SS_MAGIC_UNPACKED, 6) == 0 && sdata_size >= 32 * 1024) {
		*size = 64 * 1024;
		uint8_t *psdata = (uint8_t *) malloc(*size);

		for (size_t x = 0, y = 0; x < 32 * 1024; x++, y += 2) {
			psdata[y + 0] = 0xFF;
			psdata[y + 1] = ((const uint8_t *) sdata)[x];
		}

		return psdata;
	}

	return NULL;
}


// FileStream callbacks

void core_file_stream_write(const char *path, const void *buf, uint64_t size, void *opaque)
{
	Core *ctx = (Core *) opaque;

	const char *ext = strrchr(path, '.');
	if (!ext)
		ext = "";

	bool sav =
		strcmp(ext, ".mcr") == 0 || // PSX
		strcmp(ext, ".bkr") == 0;   // SS

	if (ext && sav) {
		if (size == ctx->sdata_size_cur && memcmp(buf, ctx->sdata, size) == 0)
			return;

		if (size > ctx->sdata_size_max) {
			ctx->sdata_size_max = size;
			ctx->sdata = realloc(ctx->sdata, ctx->sdata_size_max);
		}

		memcpy(ctx->sdata, buf, size);
		ctx->sdata_size_cur = size;
		ctx->sdata_dirty = true;

		core_log("SDATA saved (%s), size=%u\n", ext, size);
	}
}

bool core_file_stream_read(const char *path, void **buf, uint64_t *size, void *opaque)
{
	Core *ctx = (Core *) opaque;

	const char *ext = strrchr(path, '.');
	if (!ext)
		ext = "";

	bool sav =
		strcmp(ext, ".mcr") == 0 || // PSX
		strcmp(ext, ".bkr") == 0;   // SS

	if (ext && sav) {
		if (ctx->sdata) {
			*size = ctx->sdata_size_cur;
			*buf = malloc(*size);
			memcpy(*buf, ctx->sdata, *size);

			core_log("SDATA read (%s), size=%u\n", ext, *size);

			return true;
		}
	}

	return false;
}


// Core

void CoreUnloadGame(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	if (ctx->gi)
		MDFNI_CloseGame();

	delete ctx->surface;
	delete ctx->cdif;
	free(ctx->audio);
	free(ctx->line_widths);
	free(ctx->cropped);
	free(ctx->sdata);

	file_stream_set_callbacks(NULL, NULL, NULL);

	free(ctx);
	*core = NULL;

	MDFNI_Kill();
}

void CoreSetLogFunc(CoreLogFunc func, void *opaque)
{
	CORE_LOG = func;
	CORE_LOG_OPAQUE = opaque;
}

void CoreSetAudioFunc(CoreAudioFunc func, void *opaque)
{
	CORE_AUDIO = func;
	CORE_AUDIO_OPAQUE = opaque;
}

void CoreSetVideoFunc(CoreVideoFunc func, void *opaque)
{
	CORE_VIDEO = func;
	CORE_VIDEO_OPAQUE = opaque;
}

static void core_settings(CoreSystem system)
{
	switch (system) {
		case CORE_SYSTEM_PS:
			MDFNI_SetSettingB("psx.h_overscan", false);

			for (uint8_t x = 2; x < 9; x++) {
				char setting[64];
				snprintf(setting, 64, "psx.input.port%u.memcard", x);
				MDFNI_SetSettingB(setting, false);
			}
			break;
		case CORE_SYSTEM_SS:
			MDFNI_SetSetting("ss.cart", "none");
			MDFNI_SetSettingB("ss.h_blend", true);
			MDFNI_SetSettingB("ss.h_overscan", false);
			break;
	}
}

static bool core_is_cd_system(CoreSystem system)
{
	switch (system) {
		case CORE_SYSTEM_PS:
		case CORE_SYSTEM_SS:
			return true;
		case CORE_SYSTEM_TG16:
			return MDFN_IEN_PCE::PCE_IsCD;
	}

	return false;
}

Core *CoreLoadGame(CoreSystem system, const char *systemDir, const char *path,
	const void *saveData, size_t saveDataSize)
{
	if (!MDFNI_Init())
		return NULL;

	if (!MDFNI_InitFinalize(systemDir))
		return NULL;

	Core *ctx = (Core *) calloc(1, sizeof(Core));
	ctx->system = system;

	file_stream_set_callbacks(core_file_stream_write, core_file_stream_read, ctx);

	if (saveData && saveDataSize > 0) {
		ctx->sdata = core_unpack_ss_sdata(saveData, saveDataSize, &ctx->sdata_size_cur);

		if (!ctx->sdata) {
			ctx->sdata = malloc(saveDataSize);
			ctx->sdata_size_cur = saveDataSize;
			memcpy(ctx->sdata, saveData, saveDataSize);
		}

		ctx->sdata_size_max = ctx->sdata_size_cur;
	}

	core_settings(system);

	ctx->gi = MDFNI_LoadGame(NULL, &::NVFS, path, false);

	if (ctx->gi) {
		ctx->surface = new MDFN_Surface(NULL, ctx->gi->fb_width, ctx->gi->fb_height,
			ctx->gi->fb_width * 4, MDFN_PixelFormat::ABGR32_8888);
		ctx->audio = (int16_t *) calloc(CORE_AUDIO_BUF_MAX, 1);
		ctx->line_widths = (int32_t *) calloc(ctx->gi->fb_height, 4);
		ctx->cropped = (uint8_t *) calloc(ctx->gi->fb_width * ctx->gi->fb_height, 4);

		if (system == CORE_SYSTEM_SS)
			CORE_SS_RESET = MDFNI_SetInput(12, 0);

		ctx->input = MDFNI_SetInput(0, 1); // 'gamepad'

		if (core_is_cd_system(system))
			MDFNI_SetMedia(0, 2, 0, 0); // '2' means 'Tray closed'

	} else {
		CoreUnloadGame(&ctx);
	}

	return ctx;
}

double CoreGetFrameRate(Core *ctx)
{
	CoreSystem s = ctx ? ctx->system : CORE_SYSTEM_UNKNOWN;

	switch (s) {
		case CORE_SYSTEM_PS:
			return 59.95;
	}

	return 60.0;
}

float CoreGetAspectRatio(Core *ctx)
{
	CoreSystem s = ctx ? ctx->system : CORE_SYSTEM_UNKNOWN;

	switch (s) {
		case CORE_SYSTEM_PS:
			return 4.0f / 3.2f;
	}

	return 4.0f / 3.0f;
}

static void core_apply_res_hacks(int32_t *w, int32_t *x_off)
{
	switch (*w) {
		case 264:
			*w = 256;
			*x_off += 4 * 4;
			break;
		case 330:
			*w = 320;
			*x_off += 5 * 4;
			break;
		case 378:
			*w = 384;
			*x_off -= 3 * 4;
			break;
		case 528:
			*w = 512;
			*x_off += 8 * 4;
			break;
		case 660:
			*w = 640;
			*x_off += 10 * 4;
			break;
		default:
			core_log("Unhandeled width: %d\n", *w);
			break;
	}
}

void CoreRun(Core *ctx)
{
	if (!ctx)
		return;

	EmulateSpecStruct spec = {
		.surface = ctx->surface,
		.LineWidths = ctx->line_widths,
		.SoundRate = CORE_SAMPLE_RATE,
		.SoundBuf = ctx->audio,
		.SoundBufMaxSize = CORE_AUDIO_BUF_MAX / 4,
	};

	MDFNI_Emulate(&spec);

	if (CORE_VIDEO) {
		int32_t x_off = spec.DisplayRect.x * 4;
		int32_t y_off = spec.DisplayRect.y;
		int32_t w = ctx->line_widths[0] > 0 ? ctx->line_widths[0] : spec.DisplayRect.w;
		int32_t h = spec.DisplayRect.h;
		int32_t pitch = ctx->surface->pitchinpix * 4;

		if (w > 0 && h > 0) {
			if (ctx->system == CORE_SYSTEM_PS)
				core_apply_res_hacks(&w, &x_off);

			int32_t cpitch = w * 4;

			for (int32_t y = 0; y < h; y++)
				memcpy(ctx->cropped + cpitch * y, (uint8_t *) ctx->surface->pixels + pitch * (y + y_off) + x_off, cpitch);

			CORE_VIDEO(ctx->cropped, CORE_COLOR_FORMAT_RGBA, w, h, cpitch, CORE_VIDEO_OPAQUE);
		}
	}

	if (CORE_AUDIO)
		CORE_AUDIO(ctx->audio, spec.SoundBufSize, CORE_SAMPLE_RATE, CORE_AUDIO_OPAQUE);
}

void CorePauseThreads(Core *ctx, bool pause)
{
}

void *CoreGetSaveData(Core *ctx, size_t *size, CoreSaveDataType *type)
{
	if (!ctx || !ctx->sdata || !ctx->sdata_dirty)
		return NULL;

	ctx->sdata_dirty = false;

	void *sdata = core_pack_ss_sdata(ctx->sdata, ctx->sdata_size_cur, size);
	if (sdata)
		return sdata;

	*size = ctx->sdata_size_cur;

	sdata = malloc(*size);
	memcpy(sdata, ctx->sdata, *size);

	return sdata;
}

void CoreReset(Core *ctx)
{
	if (!ctx)
		return;

	if (ctx->surface && ctx->surface->pixels)
		memset(ctx->surface->pixels, 0, ctx->surface->h * ctx->surface->pitchinpix * 4);

	if (ctx->system == CORE_SYSTEM_SS) {
		*CORE_SS_RESET = 1;

		for (uint8_t x = 0; x < 3; x++)
			CoreRun(ctx);

		*CORE_SS_RESET = 0;

	} else {
		MDFNI_Reset();
	}
}

#define SET_STATE_BIT(n) \
	*s = pressed ? *s | (1 << n) : *s & ~(1 << n)

static void core_button_psx(CoreButton button, bool pressed, uint16_t *s)
{
	switch (button) {
		case CORE_BUTTON_SELECT: SET_STATE_BIT(0);  break;
		case CORE_BUTTON_START:  SET_STATE_BIT(3);  break;
		case CORE_BUTTON_DPAD_U: SET_STATE_BIT(4);  break;
		case CORE_BUTTON_DPAD_R: SET_STATE_BIT(5);  break;
		case CORE_BUTTON_DPAD_D: SET_STATE_BIT(6);  break;
		case CORE_BUTTON_DPAD_L: SET_STATE_BIT(7);  break;
		case CORE_BUTTON_L2:     SET_STATE_BIT(8);  break;
		case CORE_BUTTON_R2:     SET_STATE_BIT(9);  break;
		case CORE_BUTTON_L:      SET_STATE_BIT(10); break;
		case CORE_BUTTON_R:      SET_STATE_BIT(11); break;
		case CORE_BUTTON_Y:      SET_STATE_BIT(12); break; // Triangle
		case CORE_BUTTON_B:      SET_STATE_BIT(13); break; // Circle
		case CORE_BUTTON_A:      SET_STATE_BIT(14); break; // X
		case CORE_BUTTON_X:      SET_STATE_BIT(15); break; // Square
	}
}

static void core_button_ss(CoreButton button, bool pressed, uint16_t *s)
{
	switch (button) {
		case CORE_BUTTON_L2:     SET_STATE_BIT(0);  break; // Z
		case CORE_BUTTON_R2:     SET_STATE_BIT(1);  break; // Y
		case CORE_BUTTON_Y:      SET_STATE_BIT(2);  break; // X
		case CORE_BUTTON_R:      SET_STATE_BIT(3);  break;
		case CORE_BUTTON_DPAD_U: SET_STATE_BIT(4);  break;
		case CORE_BUTTON_DPAD_D: SET_STATE_BIT(5);  break;
		case CORE_BUTTON_DPAD_L: SET_STATE_BIT(6);  break;
		case CORE_BUTTON_DPAD_R: SET_STATE_BIT(7);  break;
		case CORE_BUTTON_A:      SET_STATE_BIT(8);  break; // B
		case CORE_BUTTON_B:      SET_STATE_BIT(9);  break; // C
		case CORE_BUTTON_X:      SET_STATE_BIT(10); break; // A
		case CORE_BUTTON_START:  SET_STATE_BIT(11); break;
		case CORE_BUTTON_L:      SET_STATE_BIT(15); break;
	}
}

static void core_button_pce(CoreButton button, bool pressed, uint16_t *s)
{
	// TODO
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx)
		return;

	uint16_t state = ctx->input[0] | ((uint16_t) ctx->input[1] << 8);

	switch (ctx->system) {
		case CORE_SYSTEM_PS: core_button_psx(button, pressed, &state); break;
		case CORE_SYSTEM_SS: core_button_ss(button, pressed, &state); break;
		case CORE_SYSTEM_TG16: core_button_pce(button, pressed, &state); break;
		default:
			state = 0;
			break;
	}

	ctx->input[0] = state & 0xFF;
	ctx->input[1] = state >> 8;
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
	// TODO
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx)
		return NULL;

	MemoryStream st(65536);

	try {
		MDFNSS_SaveSM(&st, true);

	} catch (...) {
		return NULL;
	}

	*size = st.size();

	void *buf = malloc(*size);
	memcpy(buf, st.map(), *size);

	return buf;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx)
		return false;

	MemoryStream st(size, -1);
	memcpy(st.map(), state, size);

	try {
		MDFNSS_LoadSM(&st, true);

	} catch (...) {
		return false;
	}

	return true;
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	if (!ctx)
		return false;

	delete ctx->cdif;
	ctx->cdif = CDInterface::Open(&::NVFS, path, true, 0);

	switch (ctx->system) {
		case CORE_SYSTEM_PS:
			MDFN_IEN_PSX::CDC->SetDisc(true, NULL, NULL);
			CoreRun(ctx);

			const char *disc_id = "SCEA"; // TODO Proper region
			MDFN_IEN_PSX::CDC->SetDisc(false, ctx->cdif, disc_id);
			return true;
	}

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
