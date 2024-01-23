#include "../core.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "common/byte_stream.h"
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
	bool loaded;
};

static CoreLogFunc CORE_LOG_FUNC;
static void *CORE_LOG_OPAQUE;

void audio_stream_set_func(CoreAudioFunc func, void *opaque);
void gpu_device_set_func(CoreVideoFunc func, void *opaque);

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
	core_log("%s\n", std::string(message).c_str());
}

Core *CoreLoad(const char *system_dir, const char *save_dir)
{
	Core *ctx = (Core *) calloc(1, sizeof(Core));

	Log::RegisterCallback(core_log_callback, ctx);

	std::string subdir = Path::Combine(system_dir, "duckstation");

	EmuFolders::AppRoot = subdir;
	EmuFolders::Bios = system_dir;
	EmuFolders::DataRoot = subdir;
	EmuFolders::Cache = subdir;
	EmuFolders::Cheats = subdir;
	EmuFolders::Covers = subdir;
	EmuFolders::Dumps = subdir;
	EmuFolders::GameSettings = subdir;
	EmuFolders::InputProfiles = subdir;
	EmuFolders::MemoryCards = subdir;
	EmuFolders::Resources = subdir;
	EmuFolders::SaveStates = subdir;
	EmuFolders::Screenshots = subdir;
	EmuFolders::Shaders = subdir;
	EmuFolders::Textures = subdir;
	EmuFolders::UserResources = subdir;

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

	settings->SetBoolValue("GPU", "UseThread", false);
	settings->SetStringValue("Audio", "Backend", "Null");
	settings->SetStringValue("Audio", "StretchMode", "None");
	settings->SetStringValue("CPU", "ExecutionMode", "NewRec");

	SystemBootParameters bp = {};
	bp.filename = path;
	bp.force_software_renderer = true;

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
		case CORE_BUTTON_Y: b = AnalogController::Button::Triangle; break;
		case CORE_BUTTON_B: b = AnalogController::Button::Circle; break;
		case CORE_BUTTON_A: b = AnalogController::Button::Cross; break;
		case CORE_BUTTON_X: b = AnalogController::Button::Square; break;
		default:
			return;
	}

	c->SetBindState((u32) b, pressed ? 1 : 0);
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
	// TODO AnalogController::Count
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
	// TODO

	return 60;
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
	audio_stream_set_func(func, opaque);
}

void CoreSetVideoFunc(Core *ctx, CoreVideoFunc func, void *opaque)
{
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
