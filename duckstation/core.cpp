#include "../core.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "common/byte_stream.h"
#include "common/file_system.h"
#include "common/log.h"
#include "common/path.h"
#include "core/analog_controller.h"
#include "core/host.h"
#include "core/gpu.h"
#include "core/memory_card.h"
#include "core/pad.h"
#include "core/system.h"
#include "core/settings.h"
#include "util/ini_settings_interface.h"

struct Core {
	CoreAudioFunc audio_func;
	CoreVideoFunc video_func;
	void *audio_opaque;
	void *video_opaque;
	bool loaded;
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

uint32_t audio_stream_get_sample_rate(void);
const int16_t *audio_stream_get_frames(size_t *frames);
void gpu_device_set_func(CoreVideoFunc func, void *opaque);
bool gpu_device_got_frame(void);

static std::unique_ptr<INISettingsInterface> settings;

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

static void core_log_callback(void* pUserParam, const char* channelName, const char* functionName,
	LOGLEVEL level, std::string_view message)
{
	const char *cmsg = std::string(message).c_str();

	// Filesystem failures
	if (strstr(cmsg, "does_not_exist"))
		return;

	core_log("[%s] %s\n", channelName, cmsg);
}

Core *CoreLoad(const char *system_dir, const char *save_dir)
{
	Core *ctx = (Core *) calloc(1, sizeof(Core));

	Log::RegisterCallback(core_log_callback, ctx);

	std::string subdir = Path::Combine(system_dir, "duckstation");
	std::string null_subdir = Path::Combine(system_dir, "does_not_exist");

	FileSystem::CreateDirectory(subdir.c_str(), false);

	EmuFolders::Bios = system_dir;
	EmuFolders::AppRoot = subdir;
	EmuFolders::DataRoot = subdir;
	EmuFolders::Cache = subdir;
	EmuFolders::Cheats = subdir;
	EmuFolders::Covers = subdir;
	EmuFolders::GameSettings = subdir;
	EmuFolders::InputProfiles = subdir;
	EmuFolders::Resources = subdir;
	EmuFolders::UserResources = subdir;
	EmuFolders::Dumps = null_subdir;
	EmuFolders::Screenshots = null_subdir;
	EmuFolders::MemoryCards = null_subdir;
	EmuFolders::SaveStates = null_subdir;
	EmuFolders::Shaders = null_subdir;
	EmuFolders::Textures = null_subdir;

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

bool CoreLoadGame(Core *ctx, CoreSystem system, const char *path,
	const void *save_data, size_t save_data_size)
{
	if (!ctx)
		return false;

	System::Internal::ProcessStartup();

	std::string settings_filename = Path::Combine(EmuFolders::DataRoot, "settings.ini");

	settings = std::make_unique<INISettingsInterface>(settings_filename);
	Host::Internal::SetBaseSettingsLayer(settings.get());

	settings->SetStringValue("CPU", "ExecutionMode", "NewRec");
	settings->SetStringValue("GPU", "Renderer", "Software");
	settings->SetBoolValue("GPU", "UseThread", false);
	settings->SetStringValue("Audio", "Backend", "Null");
	settings->SetStringValue("Audio", "StretchMode", "None");

	SystemBootParameters bp = {};
	bp.filename = path;

	ctx->loaded = System::BootSystem(bp);

	if (ctx->loaded && save_data) {
		MemoryCard *mc = Pad::GetMemoryCard(0);

		if (mc) {
			MemoryCardImage::DataArray& da = mc->GetData();

			if (da.size() >= save_data_size)
				std::copy_n(da.begin(), save_data_size, (uint8_t *) save_data);
		}
	}

	return ctx->loaded;
}

void CoreUnloadGame(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	if (System::IsValid())
		System::ShutdownSystem(false);

	System::Internal::ProcessShutdown();

	ctx->loaded = false;
}

void CoreReset(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	System::ResetSystem();
}

void CoreRun(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	System::DoFrameStep();
	System::Execute();

	if (!gpu_device_got_frame() && ctx->video_func) {
		uint32_t dummy[16][16] = {0};
		ctx->video_func(dummy, CORE_COLOR_FORMAT_BGRA, 16, 16, 16 * 4, ctx->video_opaque);
	}

	if (ctx->audio_func) {
		size_t frames = 0;
		const int16_t *buf = audio_stream_get_frames(&frames);

		if (frames > 0)
			ctx->audio_func(buf, frames, audio_stream_get_sample_rate(), ctx->audio_opaque);
	}
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || !ctx->loaded)
		return;

	Controller *c = System::GetController(player);
	if (!c)
		return;

	AnalogController::Button b;

	switch (button) {
		case CORE_BUTTON_SELECT: b = AnalogController::Button::Select; break;
		case CORE_BUTTON_L3: b = AnalogController::Button::L3; break;
		case CORE_BUTTON_R3: b = AnalogController::Button::R3; break;
		case CORE_BUTTON_START: b = AnalogController::Button::Start; break;
		case CORE_BUTTON_DPAD_U: b = AnalogController::Button::Up; break;
		case CORE_BUTTON_DPAD_R: b = AnalogController::Button::Right; break;
		case CORE_BUTTON_DPAD_D: b = AnalogController::Button::Down; break;
		case CORE_BUTTON_DPAD_L: b = AnalogController::Button::Left; break;
		case CORE_BUTTON_L2: b = AnalogController::Button::L2; break;
		case CORE_BUTTON_R2: b = AnalogController::Button::R2; break;
		case CORE_BUTTON_L: b = AnalogController::Button::L1; break;
		case CORE_BUTTON_R: b = AnalogController::Button::R1; break;
		case CORE_BUTTON_X: b = AnalogController::Button::Triangle; break;
		case CORE_BUTTON_A: b = AnalogController::Button::Circle; break;
		case CORE_BUTTON_B: b = AnalogController::Button::Cross; break;
		case CORE_BUTTON_Y: b = AnalogController::Button::Square; break;
		default:
			return;
	}

	c->SetBindState((u32) b, pressed ? 1 : 0);
}

static void core_set_axis(AnalogController::HalfAxis neg, AnalogController::HalfAxis pos,
	Controller *c, int16_t value)
{
	if (value < 0) {
		c->SetBindState((u32) AnalogController::Button::Count + (u32) neg, (float) value / INT16_MIN);
		c->SetBindState((u32) AnalogController::Button::Count + (u32) pos, 0);

	} else {
		c->SetBindState((u32) AnalogController::Button::Count + (u32) neg, 0);
		c->SetBindState((u32) AnalogController::Button::Count + (u32) pos, (float) value / INT16_MAX);
	}
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
	if (!ctx || !ctx->loaded)
		return;

	Controller *c = System::GetController(player);
	if (!c)
		return;

	switch (axis) {
		case CORE_AXIS_LX:
			core_set_axis(AnalogController::HalfAxis::LLeft, AnalogController::HalfAxis::LRight, c, value);
			break;
		case CORE_AXIS_LY:
			core_set_axis(AnalogController::HalfAxis::LUp, AnalogController::HalfAxis::LDown, c, value);
			break;
		case CORE_AXIS_RX:
			core_set_axis(AnalogController::HalfAxis::RLeft, AnalogController::HalfAxis::RRight, c, value);
			break;
		case CORE_AXIS_RY:
			core_set_axis(AnalogController::HalfAxis::RUp, AnalogController::HalfAxis::RDown, c, value);
			break;
		default:
			break;
	}
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	std::unique_ptr<GrowableMemoryByteStream> ms =
		ByteStream::CreateGrowableMemoryStream();

	if (!System::SaveStateToStream(ms.get(), 0))
		return NULL;

	*size = ms->GetMemorySize();

	void *state = malloc(*size);
	memcpy(state, ms->GetMemoryPointer(), *size);

	return state;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx || !ctx->loaded)
		return false;

	std::unique_ptr<MemoryByteStream> ms =
		ByteStream::CreateMemoryStream((void *) state, size);

	return System::LoadStateFromStream(ms.get(), false);
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	if (!ctx || !ctx->loaded)
		return false;

	return System::InsertMedia(path);
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	MemoryCard *mc = Pad::GetMemoryCard(0);
	if (!mc)
		return NULL;

	MemoryCardImage::DataArray& da = mc->GetData();

	*size = da.size();

	uint8_t *sd = (uint8_t *) malloc(*size);
	std::copy_n(sd, *size, da.begin());

	return sd;
}

bool CoreGameIsLoaded(Core *ctx)
{
	return ctx ? ctx->loaded : false;
}

double CoreGetFrameRate(Core *ctx)
{
	return 59.94;
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx || !ctx->loaded || !g_gpu)
		return 4.0f / 3.0f;

	return g_gpu->ComputeDisplayAspectRatio();
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

	gpu_device_set_func(func, opaque);
}

const CoreSetting *CoreGetAllSettings(Core *ctx, uint32_t *len)
{
	// TODO

	*len = 0;

	return NULL;
}

void CoreSetSetting(Core *ctx, const char *key, const char *val)
{
	// TODO

	if (!ctx)
		return;
}

const char *CoreGetSetting(Core *ctx, const char *key)
{
	// TODO

	if (!ctx)
		return NULL;

	return NULL;
}

void CoreResetSettings(Core *ctx)
{
	// TODO

	if (!ctx)
		return;
}
