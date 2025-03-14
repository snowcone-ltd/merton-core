#include "../core.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "common/byte_stream.h"
#include "common/file_system.h"
#include "common/layered_settings_interface.h"
#include "common/log.h"
#include "common/path.h"
#include "common/error.h"
#include "core/analog_controller.h"
#include "core/host.h"
#include "core/gpu.h"
#include "core/memory_card.h"
#include "core/pad.h"
#include "core/system.h"
#include "core/settings.h"
#include "util/ini_settings_interface.h"

struct Core {
	std::unique_ptr<INISettingsInterface> settings;
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

void audio_stream_set_func(CoreAudioFunc func, void *opaque);
void gpu_device_set_func(CoreVideoFunc func, void *opaque);
void gpu_device_finish(void);
void audio_stream_finish(void);

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
	std::string str = std::string(message);
	const char *cmsg = str.c_str();

	// Filesystem failures
	if (strstr(cmsg, "does_not_exist"))
		return;

	// Searching for BIOS
	if (!strcmp(channelName, "BIOS") && strstr(cmsg, "Skipping"))
		return;

	// Low level
	if (!strcmp(channelName, "Bus"))
		return;

	// Memory card
	if (!strcmp(channelName, "Pad"))
		return;

	core_log("[%s] %s\n", channelName, cmsg);
}

void CoreUnloadGame(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	if (System::IsValid())
		System::ShutdownSystem(false);

	System::Internal::CPUThreadShutdown();

	if (ctx->settings)
		ctx->settings.reset();

	Log::UnregisterCallback(core_log_callback, ctx);

	free(ctx);
	*core = NULL;
}

Core *CoreLoadGame(CoreSystem system, const char *systemDir, const char *path,
	const void *saveData, size_t saveDataSize)
{
	Core *ctx = (Core *) calloc(1, sizeof(Core));

	Log::RegisterCallback(core_log_callback, ctx);

	std::string subdir = Path::Combine(systemDir, "does_not_exist");

	EmuFolders::Bios = systemDir;
	EmuFolders::AppRoot = subdir;
	EmuFolders::DataRoot = subdir;
	EmuFolders::Cache = subdir;
	EmuFolders::Cheats = subdir;
	EmuFolders::Covers = subdir;
	EmuFolders::GameSettings = subdir;
	EmuFolders::InputProfiles = subdir;
	EmuFolders::Resources = subdir;
	EmuFolders::UserResources = subdir;
	EmuFolders::Dumps = subdir;
	EmuFolders::Screenshots = subdir;
	EmuFolders::MemoryCards = subdir;
	EmuFolders::SaveStates = subdir;
	EmuFolders::Shaders = subdir;
	EmuFolders::Textures = subdir;

	std::string settings_filename = Path::Combine(EmuFolders::DataRoot, "settings.ini");

	ctx->settings = std::make_unique<INISettingsInterface>(settings_filename);

	LayeredSettingsInterface *si = (LayeredSettingsInterface *) Host::GetSettingsInterface();
	si->SetLayer(LayeredSettingsInterface::LAYER_BASE, ctx->settings.get());

	ctx->settings->SetStringValue("CPU", "ExecutionMode", "NewRec");
	ctx->settings->SetStringValue("GPU", "Renderer", "Software");
	ctx->settings->SetBoolValue("GPU", "UseThread", false);
	ctx->settings->SetStringValue("Audio", "Backend", "Null");
	ctx->settings->SetStringValue("Audio", "StretchMode", "None");
	ctx->settings->SetBoolValue("Logging", "LogToConsole", false);

	Error error;
	System::Internal::CPUThreadInitialize(&error);

	SystemBootParameters bp = {};
	bp.filename = path;

	bool loaded = System::BootSystem(bp, &error);

	if (loaded && saveData) {
		MemoryCard *mc = Pad::GetMemoryCard(0);

		if (mc) {
			MemoryCardImage::DataArray& da = mc->GetData();

			if (da.size() >= saveDataSize)
				std::copy_n((uint8_t *) saveData, saveDataSize, da.begin());
		}
	}

	if (!loaded)
		CoreUnloadGame(&ctx);

	return ctx;
}

void CoreReset(Core *ctx)
{
	if (!ctx)
		return;

	System::ResetSystem();
}

void CoreRun(Core *ctx)
{
	if (!ctx)
		return;

	System::DoFrameStep();
	System::Execute();

	gpu_device_finish();
	audio_stream_finish();
}

void CorePauseThreads(Core *ctx, bool pause)
{
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx)
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
		case CORE_BUTTON_X: b = AnalogController::Button::Square; break;
		case CORE_BUTTON_A: b = AnalogController::Button::Cross; break;
		case CORE_BUTTON_B: b = AnalogController::Button::Circle; break;
		case CORE_BUTTON_Y: b = AnalogController::Button::Triangle; break;
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
	if (!ctx)
		return;

	Controller *c = System::GetController(player);
	if (!c)
		return;

	switch (axis) {
		case CORE_AXIS_LX:
			core_set_axis(AnalogController::HalfAxis::LLeft, AnalogController::HalfAxis::LRight, c, value);
			break;
		case CORE_AXIS_LY:
			core_set_axis(AnalogController::HalfAxis::LDown, AnalogController::HalfAxis::LUp, c, value);
			break;
		case CORE_AXIS_RX:
			core_set_axis(AnalogController::HalfAxis::RLeft, AnalogController::HalfAxis::RRight, c, value);
			break;
		case CORE_AXIS_RY:
			core_set_axis(AnalogController::HalfAxis::RDown, AnalogController::HalfAxis::RUp, c, value);
			break;
		default:
			break;
	}
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx)
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
	if (!ctx)
		return false;

	std::unique_ptr<MemoryByteStream> ms =
		ByteStream::CreateMemoryStream((void *) state, size);

	Error error;

	return System::LoadStateFromStream(ms.get(), &error, false);
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	if (!ctx)
		return false;

	return System::InsertMedia(path);
}

void *CoreGetSaveData(Core *ctx, size_t *size, CoreSaveDataType *type)
{
	if (!ctx)
		return NULL;

	MemoryCard *mc = Pad::GetMemoryCard(0);
	if (!mc)
		return NULL;

	MemoryCardImage::DataArray& da = mc->GetData();

	*size = da.size();

	uint8_t *sd = (uint8_t *) malloc(*size);
	std::copy_n(da.begin(), *size, sd);

	return sd;
}

double CoreGetFrameRate(Core *ctx)
{
	return 59.95;
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx || !g_gpu)
		return 4.0f / 3.0f;

	return g_gpu->ComputeDisplayAspectRatio();
}

void CoreSetLogFunc(CoreLogFunc func, void *opaque)
{
	CORE_LOG_FUNC = func;
	CORE_LOG_OPAQUE = opaque;
}

void CoreSetAudioFunc(CoreAudioFunc func, void *opaque)
{
	audio_stream_set_func(func, opaque);
}

void CoreSetVideoFunc(CoreVideoFunc func, void *opaque)
{
	gpu_device_set_func(func, opaque);
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
