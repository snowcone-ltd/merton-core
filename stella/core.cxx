#include "../core.h"

#include <stdlib.h>
#include <stdarg.h>

#include "EventHandler.hxx"
#include "FBSurface.hxx"
#include "M6532.hxx"
#include "PaletteHandler.hxx"
#include "StateManager.hxx"
#include "Switches.hxx"
#include "TIA.hxx"
#include "TIASurface.hxx"

#include "shim/SoundCore.hxx"
#include "osystem-core.hxx"

#define SAMPLE_RATE ((262 * 76 * 60) / 38.0)

struct Core {
	bool loaded;
	bool crop_overscan;
	CoreVideoFunc video_func;
	CoreAudioFunc audio_func;
	void *video_opaque;
	void *audio_opaque;

	unique_ptr<OSystemCore> sys;
	uint8_t ram[128];
	int16_t audio_buf[(31440 / 50 * 4 * 5) / 4];
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

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

Core *CoreLoad(const char *system_dir)
{
	return (Core *) calloc(1, sizeof(Core));
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

static void core_show_logs(void)
{
	string messages = Logger::instance().logMessages();

	if (messages.length() > 0)
		core_log("%s", messages.c_str());
}

bool CoreLoadGame(Core *ctx, CoreSystem system, const char *path,
	const void *save_data, size_t save_data_size)
{
	if (!ctx)
		return false;

	ctx->sys = make_unique<OSystemCore>();

	Settings::Options options;
	ctx->sys->initialize(options);

	Settings& settings = ctx->sys->settings();
	settings.setValue("loglevel", 999);
	settings.setValue("logtoconsole", false);
	settings.setValue("speed", 1.0);
	settings.setValue("uimessages", false);
	settings.setValue("format", "AUTO");
	settings.setValue("palette", PaletteHandler::SETTING_STANDARD);
	settings.setValue("tia.zoom", 1);
	settings.setValue("tia.vsizeadjust", 0);
	settings.setValue("tia.inter", false);
	settings.setValue("tv.filter", static_cast<int>(NTSCFilter::Preset::OFF));
	settings.setValue("tv.phosphor", "byrom");
	settings.setValue("tv.phosblend", 60);
	settings.setValue(AudioSettings::SETTING_PRESET, static_cast<int>(AudioSettings::Preset::custom));
	settings.setValue(AudioSettings::SETTING_SAMPLE_RATE, SAMPLE_RATE);
	settings.setValue(AudioSettings::SETTING_FRAGMENT_SIZE, 128);
	settings.setValue(AudioSettings::SETTING_BUFFER_SIZE, 8);
	settings.setValue(AudioSettings::SETTING_HEADROOM, 0);
	settings.setValue(AudioSettings::SETTING_RESAMPLING_QUALITY,
		static_cast<int>(AudioSettings::ResamplingQuality::nearestNeightbour));
	settings.setValue(AudioSettings::SETTING_VOLUME, 100);
	settings.setValue(AudioSettings::SETTING_STEREO, "byrom");

	FSNode rom(path);

	ctx->loaded = ctx->sys->createConsole(rom) == EmptyString;

	core_show_logs();

	return ctx->loaded;
}

void CoreUnloadGame(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	if (ctx->sys)
		ctx->sys.reset();

	ctx->loaded = false;
}

void CoreReset(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->sys->console().system().reset();
}

void CoreRun(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	for (int lcv = 0; lcv <= 127; lcv++)
		ctx->sys->console().system().m6532().poke(lcv | 0x80, ctx->ram[lcv]);

	const Console& console = ctx->sys->console();
	TIA& tia = console.tia();

	// Input
	console.leftController().update();
	console.rightController().update();
	console.switches().update();

	// Video
	do {
		tia.updateScanline();
	} while (tia.scanlines() != 0);

	if (tia.newFramePending()) {
		FrameBuffer& frame = ctx->sys->frameBuffer();

		tia.renderToFrameBuffer();
		frame.updateInEmulationMode(0);

		if (ctx->video_func) {
			uint8_t zoom = frame.tiaSurface().ntscEnabled() ? 2 : 1;
			uint32_t max_w = AtariNTSC::outWidth(160);
			uint32_t h = console.tia().height();
			uint32_t pitch = max_w * 4;
			uint32_t w = zoom == 1 ? console.tia().width() : max_w;

			uint32_t crop_left = ctx->crop_overscan ? (zoom == 2 ? 26 : 8) : 0;

			const FBSurface& surface = frame.tiaSurface().tiaSurface();

			uInt32 _pitch = 0;
			uint32_t *frame_ptr = NULL;
			surface.basePtr(frame_ptr, _pitch);

			ctx->video_func(frame_ptr + crop_left, CORE_COLOR_FORMAT_BGRA, w - crop_left, h, pitch, ctx->video_opaque);
		}
	}

	// Audio
	if (ctx->audio_func) {
		uInt32 audio_samples = 0;
		static_cast<SoundCore&>(ctx->sys->sound()).dequeue(ctx->audio_buf, &audio_samples);

		if (audio_samples > 0)
			ctx->audio_func(ctx->audio_buf, audio_samples, SAMPLE_RATE, ctx->audio_opaque);
	}

	memcpy(ctx->ram, console.system().m6532().getRAM(), 128);
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || !ctx->loaded)
		return;

	Event::Type e;

	switch (button) {
		// Controller::Type::Joystick
		case CORE_BUTTON_A:      e = Event::LeftJoystickFire;  break;
		case CORE_BUTTON_DPAD_D: e = Event::LeftJoystickDown;  break;
		case CORE_BUTTON_DPAD_U: e = Event::LeftJoystickUp;    break;
		case CORE_BUTTON_DPAD_L: e = Event::LeftJoystickLeft;  break;
		case CORE_BUTTON_DPAD_R: e = Event::LeftJoystickRight; break;

		// All controllers
		case CORE_BUTTON_L:      e = Event::ConsoleLeftDiffA;  break;
		case CORE_BUTTON_L2:     e = Event::ConsoleLeftDiffB;  break;
		case CORE_BUTTON_L3:     e = Event::ConsoleColor;      break;
		case CORE_BUTTON_R:      e = Event::ConsoleRightDiffA; break;
		case CORE_BUTTON_R2:     e = Event::ConsoleRightDiffB; break;
		case CORE_BUTTON_R3:     e = Event::ConsoleBlackWhite; break;
		case CORE_BUTTON_SELECT: e = Event::ConsoleSelect;     break;
		case CORE_BUTTON_START:  e = Event::ConsoleReset;      break;
		default:
			return;
	}

	ctx->sys->eventHandler().handleEvent(e, pressed ? 1 : 0);
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
	// May need this For different controller types
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	Serializer s;
	if (!ctx->sys->state().saveState(s))
		return NULL;

	*size = s.size();
	uint8_t *state = (uint8_t *) malloc(*size);

	s.getByteArray(state, *size);

	return state;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx || !ctx->loaded)
		return false;

	Serializer s;
	s.putByteArray((const uint8_t *) state, size);

	if (!ctx->sys->state().loadState(s))
		return false;

	memcpy(ctx->ram, ctx->sys->console().system().m6532().getRAM(), 128);

	return true;
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	return NULL;
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	return false;
}

bool CoreGameIsLoaded(Core *ctx)
{
	return ctx ? ctx->loaded : false;
}

static bool core_is_ntsc(Core *ctx)
{
	return ctx->sys->console().gameRefreshRate() == 60;
}

double CoreGetFrameRate(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 60;

	return core_is_ntsc(ctx) ? 60 : 50;
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 1;

	const Console& console = ctx->sys->console();
	FrameBuffer& frame = ctx->sys->frameBuffer();

	NTSCFilter::Preset video_filter = NTSCFilter::Preset::OFF;

	uint8_t zoom = frame.tiaSurface().ntscEnabled() ? 2 : 1;

	uint32_t crop_left = ctx->crop_overscan ? (zoom == 2 ? 26 : 8) : 0;

	uint32_t base_h = console.tia().height() * zoom;
	uint32_t max_w = AtariNTSC::outWidth(160);
	uint32_t base_w = (zoom == 1 ? console.tia().width() * 2 : max_w) -
		crop_left * (zoom == 1 ? 2 : 1);

	double par = video_filter == NTSCFilter::Preset::OFF ? 1.0 :
		core_is_ntsc(ctx) ? (6.1363635 / 3.579545454) / 2.0 :
		(7.3750000 / (4.43361875 * 4.0 / 5.0)) / 2.0;

	return par * base_w / base_h;
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

const CoreSetting *CoreGetAllSettings(Core *ctx, uint32_t *len)
{
	// TODO

	return NULL;
}

void CoreSetSetting(Core *ctx, const char *key, const char *val)
{
	// TODO
}

const char *CoreGetSetting(Core *ctx, const char *key)
{
	// TODO

	return NULL;
}

void CoreResetSettings(Core *ctx)
{
	// TODO
}
