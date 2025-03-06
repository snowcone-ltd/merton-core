#include "mednafen.h"
#include "state-driver.h"
#include "netplay-driver.h"
#include "MemoryStream.h"

#include "../core.h"

#include <stdlib.h>

struct Core {
	Mednafen::MDFNGI *gi;
	Mednafen::MDFN_Surface *surface;
	uint8_t *cropped;
	uint8_t *input;
	int16_t *audio;
	int32_t *line_widths;
};

static CoreLogFunc CORE_LOG;
static void *CORE_LOG_OPAQUE;

static CoreVideoFunc CORE_VIDEO;
static void *CORE_VIDEO_OPAQUE;

static CoreAudioFunc CORE_AUDIO;
static void *CORE_AUDIO_OPAQUE;

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
	core_log("[Notice] %s\n", s);
}

void Mednafen::MDFND_OutputInfo(const char *s) noexcept
{
	core_log("[Info] %s", s);
}

void Mednafen::MDFND_SetStateStatus(StateStatusStruct *status) noexcept
{
}

void Mednafen::MDFND_NetplayText(const char* text, bool NetEcho)
{
}

void Mednafen::MDFND_MediaSetNotification(uint32 drive_idx, uint32 state_idx, uint32 media_idx, uint32 orientation_idx)
{
	core_log("[Media] drive_idx=%u, state_idx=%u, media_idx=%u\n", drive_idx, state_idx, media_idx);
}

void Mednafen::MDFND_MidSync(EmulateSpecStruct *espec, const unsigned flags)
{
}

void Mednafen::MDFND_SetMovieStatus(StateStatusStruct *status) noexcept
{
}


// Core

void CoreUnloadGame(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	if (ctx->gi)
		Mednafen::MDFNI_CloseGame();

	delete ctx->surface;
	free(ctx->audio);
	free(ctx->line_widths);
	free(ctx->cropped);

	Mednafen::MDFNI_Kill();

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
	CORE_AUDIO = func;
	CORE_AUDIO_OPAQUE = opaque;
}

void CoreSetVideoFunc(CoreVideoFunc func, void *opaque)
{
	CORE_VIDEO = func;
	CORE_VIDEO_OPAQUE = opaque;
}

Core *CoreLoadGame(CoreSystem system, const char *systemDir, const char *path,
	const void *saveData, size_t saveDataSize)
{
	if (!Mednafen::MDFNI_Init())
		return NULL;

	if (!Mednafen::MDFNI_InitFinalize(systemDir))
		return NULL;

	Core *ctx = (Core *) calloc(1, sizeof(Core));
	ctx->gi = Mednafen::MDFNI_LoadGame(NULL, &::Mednafen::NVFS, path, false);

	if (ctx->gi) {
		ctx->surface = new Mednafen::MDFN_Surface(NULL, ctx->gi->fb_width, ctx->gi->fb_height,
			ctx->gi->fb_width * 4, Mednafen::MDFN_PixelFormat::ABGR32_8888);
		ctx->audio = (int16_t *) calloc(10 * 1024 * 1024, 1);
		ctx->line_widths = (int32_t *) calloc(ctx->gi->fb_height, 4);
		ctx->cropped = (uint8_t *) calloc(ctx->gi->fb_width * ctx->gi->fb_height, 4);

		ctx->input = Mednafen::MDFNI_SetInput(0, 1); // PSX 'gamepad'
		Mednafen::MDFNI_SetMedia(0, 2, 0, 0); // '2' means 'Tray closed'

	} else {
		CoreUnloadGame(&ctx);
	}

	return ctx;
}

double CoreGetFrameRate(Core *ctx)
{
	// TODO

	return 60;
}

float CoreGetAspectRatio(Core *ctx)
{
	// TODO

	return 4.0f / 3.0f;
}

void CoreRun(Core *ctx)
{
	if (!ctx)
		return;

	Mednafen::EmulateSpecStruct spec = {
		.surface = ctx->surface,
		.LineWidths = ctx->line_widths,
		.SoundRate = 48000,
		.SoundBuf = ctx->audio,
		.SoundBufMaxSize = (10 * 1024 * 1024) / 4,
	};

	Mednafen::MDFNI_Emulate(&spec);

	if (CORE_VIDEO) {
		int32_t x_off = spec.DisplayRect.x * 4;
		int32_t y_off = spec.DisplayRect.y;
		int32_t w = spec.DisplayRect.w > 0 ? spec.DisplayRect.w : ctx->line_widths[0];
		int32_t h = spec.DisplayRect.h;
		int32_t pitch = ctx->surface->pitchinpix * 4;
		int32_t cpitch = w * 4;

		for (int32_t y = y_off; y < h; y++)
			memcpy(ctx->cropped + cpitch * y, (uint8_t *) ctx->surface->pixels + pitch * y + x_off, cpitch);

		CORE_VIDEO(ctx->cropped, CORE_COLOR_FORMAT_RGBA, w, h, cpitch, CORE_VIDEO_OPAQUE);
	}

	if (CORE_AUDIO)
		CORE_AUDIO(ctx->audio, spec.SoundBufSize, 48000, CORE_AUDIO_OPAQUE);
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	// TODO

	if (!ctx)
		return NULL;

	return NULL;
}

void CoreReset(Core *ctx)
{
	if (!ctx)
		return;

	if (ctx->surface && ctx->surface->pixels)
		memset(ctx->surface->pixels, 0, ctx->surface->h * ctx->surface->pitchinpix * 4);

	Mednafen::MDFNI_Reset();
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx)
		return;

	uint16_t state = ctx->input[0] | ((uint16_t) ctx->input[1] << 8);

	#define SET_STATE_BIT(n) \
		state = pressed ? state | (1 << n) : state & ~(1 << n)

	switch (button) {
		case CORE_BUTTON_SELECT: SET_STATE_BIT(0); break;
		case CORE_BUTTON_L3: SET_STATE_BIT(1); break;
		case CORE_BUTTON_R3: SET_STATE_BIT(2); break;
		case CORE_BUTTON_START: SET_STATE_BIT(3); break;
		case CORE_BUTTON_DPAD_U: SET_STATE_BIT(4); break;
		case CORE_BUTTON_DPAD_R: SET_STATE_BIT(5); break;
		case CORE_BUTTON_DPAD_D: SET_STATE_BIT(6); break;
		case CORE_BUTTON_DPAD_L: SET_STATE_BIT(7); break;
		case CORE_BUTTON_L2: SET_STATE_BIT(8); break;
		case CORE_BUTTON_R2: SET_STATE_BIT(9); break;
		case CORE_BUTTON_L: SET_STATE_BIT(10); break;
		case CORE_BUTTON_R: SET_STATE_BIT(11); break;
		case CORE_BUTTON_Y: SET_STATE_BIT(12); break;
		case CORE_BUTTON_B: SET_STATE_BIT(13); break;
		case CORE_BUTTON_A: SET_STATE_BIT(14); break;
		case CORE_BUTTON_X: SET_STATE_BIT(15); break;
		default:
			return;
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

	Mednafen::MemoryStream st(65536);

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

	Mednafen::MemoryStream st(size, -1);
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
	// TODO

	if (!ctx)
		return false;

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
