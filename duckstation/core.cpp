#include "../core.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "core/system.h"
#include "core/settings.h"
#include "core/host.h"
#include "common/log.h"
#include "common/path.h"
#include "util/ini_settings_interface.h"

#include "matoya.h"

struct Core {
	bool loaded;
	MTY_Thread *thread;
	char *path;
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

static std::unique_ptr<INISettingsInterface> settings;
static constexpr u32 SETTINGS_VERSION = 3;

bool CoreLoadGame(Core *ctx, CoreSystem system, const char *path,
	const void *save_data, size_t save_data_size)
{
	if (!ctx)
		return false;

	ctx->path = MTY_Strdup(path);

	System::Internal::ProcessStartup();

	std::string settings_filename = Path::Combine(EmuFolders::DataRoot, "settings.ini");

	settings = std::make_unique<INISettingsInterface>(settings_filename);
	Host::Internal::SetBaseSettingsLayer(settings.get());

	settings->SetBoolValue("GPU", "UseThread", false);
	settings->SetStringValue("Audio", "Backend", "Null");
	settings->SetStringValue("CPU", "ExecutionMode", "NewRec");

	SystemBootParameters bp = {};
	bp.filename = ctx->path;
	bp.force_software_renderer = true;
	bp.load_image_to_ram = true;

	ctx->loaded = System::BootSystem(bp);

	return ctx->loaded;
}

void CoreUnloadGame(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	if (System::IsValid())
		System::ShutdownSystem(false);

	System::Internal::ProcessShutdown();
}

void CoreReset(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;
}

static void *core_emulator_thread(void *opaque)
{
	Core *ctx = (Core *) opaque;

	return NULL;
}

void CoreRun(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	//if (!ctx->thread)
	//	ctx->thread = MTY_ThreadCreate(core_emulator_thread, ctx);

	core_log("EXECUTE START\n");

	System::DoFrameStep();
	System::Execute();

	core_log("EXECUTE DONE\n");
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || !ctx->loaded)
		return;
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	return NULL;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx || !ctx->loaded)
		return false;

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

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	return NULL;
}

bool CoreGameIsLoaded(Core *ctx)
{
	return ctx ? ctx->loaded : false;
}

double CoreGetFrameRate(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 60;

	return 60;
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 1;

	return 1;
}

void CoreSetLogFunc(Core *ctx, CoreLogFunc func, void *opaque)
{
	CORE_LOG_FUNC = func;
	CORE_LOG_OPAQUE = opaque;
}

void CoreSetAudioFunc(Core *ctx, CoreAudioFunc func, void *opaque)
{
}

void CoreSetVideoFunc(Core *ctx, CoreVideoFunc func, void *opaque)
{
}

const CoreSetting *CoreGetAllSettings(Core *ctx, uint32_t *len)
{
	*len = 0;

	return NULL;
}

void CoreSetSetting(Core *ctx, const char *key, const char *val)
{
	if (!ctx)
		return;
}

const char *CoreGetSetting(Core *ctx, const char *key)
{
	if (!ctx)
		return NULL;

	return NULL;
}

void CoreResetSettings(Core *ctx)
{
	if (!ctx)
		return;
}
