#include "core.h"

#include <stdlib.h>

#include "Core/NES/GameDatabase.h"
#include "Core/Shared/Emulator.h"
#include "Core/Shared/EmuSettings.h"
#include "Core/Shared/KeyManager.h"
#include "Core/Shared/Video/BaseVideoFilter.h"
#include "Core/Shared/MessageManager.h"
#include "Utilities/VirtualFile.h"
#include "Utilities/FolderUtilities.h"

#include "Shim/ShimKeyManager.h"

#include "MesenDB.inc"

#include "palette.h"

#if defined(_MSC_VER)
	#define strdup _strdup
#endif

struct Core {
	Emulator *emu;
	CoreLogFunc log_func;
	CoreVideoFunc video_func;
	BaseVideoFilter *filter;
	ShimKeyManager *km;
	void *log_opaque;
	void *video_opaque;
	void *audio_opaque;
	uint8_t pr_initial[0x20];
	bool pr_written;
	bool loaded;

	struct {
		bool wait_for_pr;
	} settings;
};


// Shimmed

void battery_manager_set_save_data(const void *save_data, size_t save_data_size);
const void *battery_manager_get_save_data(size_t *save_data_size);
void emulator_set_system(CoreSystem system);
void sound_mixer_set_audio_func(CoreAudioFunc func, void *opaque);


// API

static void core_reset_settings(Core *ctx);

Core *CoreLoad(const char *system_dir, const char *save_dir)
{
	Core *ctx = (Core *) calloc(1, sizeof(Core));

	std::stringstream db;
	db.write((const char *) MesenDatabase, sizeof(MesenDatabase));
	GameDatabase::LoadGameDb(db);
	MessageManager::ClearLog();

	FolderUtilities::SetFolderOverrides("", "", "", system_dir);

	ctx->emu = new Emulator();

	KeyManager::SetSettings(ctx->emu->GetSettings());

	ctx->km = new ShimKeyManager(ctx->emu);
	KeyManager::RegisterKeyManager(ctx->km);

	core_reset_settings(ctx);

	return ctx;
}

void CoreUnload(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	CoreUnloadGame(ctx);

	if (ctx->filter)
		delete ctx->filter;

	if (ctx->km)
		delete ctx->km;

	if (ctx->emu)
		delete ctx->emu;

	sound_mixer_set_audio_func(NULL, NULL);

	free(ctx);
	*core = NULL;
}

static void core_get_logs(CoreLogFunc func, void *opaque)
{
	if (!func)
		return;

	string log = MessageManager::GetLog();
	if (log.empty())
		return;

	func(log.c_str(), opaque);
	MessageManager::ClearLog();
}

bool CoreLoadGame(Core *ctx, CoreSystem system, const char *path,
	const void *save_data, size_t save_data_size)
{
	if (!ctx || !ctx->emu)
		return false;

	CoreUnloadGame(ctx);

	battery_manager_set_save_data(save_data, save_data_size);
	emulator_set_system(system);

	VirtualFile vfile = VirtualFile(path);

	ctx->loaded = ctx->emu->LoadRom(vfile, {});

	if (ctx->loaded) {
		ctx->filter = ctx->emu->GetVideoFilter(true);

		if (system == CORE_SYSTEM_NES) {
			ConsoleMemoryInfo mi = ctx->emu->GetMemory(MemoryType::NesPaletteRam);
			memcpy(ctx->pr_initial, mi.Memory, 0x20);

		} else {
			ctx->pr_written = true;
		}
	}

	core_get_logs(ctx->log_func, ctx->log_opaque);

	return ctx->loaded;
}

void CoreUnloadGame(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->loaded = false;
	ctx->pr_written = false;
	ctx->emu->Stop(false);

	battery_manager_set_save_data(NULL, 0);
}

void CoreReset(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->emu->Reset();
}

static bool core_palette_ram_written(Core *ctx)
{
	if (!ctx->settings.wait_for_pr)
		return true;

	if (!ctx->pr_written && ctx->emu->GetConsoleType() == ConsoleType::Nes) {
		ConsoleMemoryInfo mi = ctx->emu->GetMemory(MemoryType::NesPaletteRam);
		ctx->pr_written = memcmp(ctx->pr_initial, mi.Memory, 0x20);
	}

	return ctx->pr_written;
}

static void core_get_frame(Core *ctx, CoreVideoFunc func, void *opaque)
{
	if (!func)
		return;

	if (!core_palette_ram_written(ctx))
		return;

	PpuFrameInfo pi = ctx->emu->GetPpuFrame();
	ctx->filter->SetBaseFrameInfo({pi.Width, pi.Height});
	FrameInfo fi = ctx->filter->SendFrame((uint16_t *) pi.FrameBuffer, 0, 0, nullptr, true);

	uint32_t *frame = ctx->filter->GetOutputBuffer();

	func(frame, CORE_COLOR_FORMAT_BGRA, fi.Width, fi.Height, fi.Width * 4, opaque);
}

void CoreRun(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->emu->Run();

	core_get_frame(ctx, ctx->video_func, ctx->video_opaque);
	core_get_logs(ctx->log_func, ctx->log_opaque);
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || !ctx->loaded)
		return;

	ctx->km->SetButton(player, button, pressed);
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx || !ctx->loaded)
		return NULL;

	std::stringstream ss;
	ctx->emu->Serialize(ss, false, 0);

	ss.seekg(0, ios::end);
	*size = ss.tellg();

	char *buf = (char *) malloc(*size);
	memcpy(buf, ss.str().c_str(), *size);

	return buf;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx || !ctx->loaded)
		return false;

	std::stringstream ss;
	ss.write((char *) state, size);

	return ctx->emu->Deserialize(ss, 0, false, std::optional<ConsoleType>());
}

uint8_t CoreGetNumDisks(Core *ctx)
{
	// Mesen prefers auto loading FDS disks (better)

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

	ctx->emu->GetConsole()->SaveBattery();

	void *copy = NULL;
	const void *save_data = battery_manager_get_save_data(size);

	if (save_data && *size > 0) {
		copy = malloc(*size);
		memcpy(copy, save_data, *size);
	}

	return copy;
}

bool CoreGameIsLoaded(Core *ctx)
{
	return ctx ? ctx->loaded : false;
}

double CoreGetFrameRate(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 60;

	return ctx->emu->GetFps();
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx || !ctx->loaded)
		return 1;

	PpuFrameInfo fi = ctx->emu->GetPpuFrame();

	return ctx->emu->GetSettings()->GetAspectRatio(ctx->emu->GetRegion(), {fi.Width, fi.Height});
}

void CoreSetLogFunc(Core *ctx, CoreLogFunc func, void *opaque)
{
	ctx->log_func = func;
	ctx->log_opaque = opaque;
}

void CoreSetAudioFunc(Core *ctx, CoreAudioFunc func, void *opaque)
{
	sound_mixer_set_audio_func(func, opaque);
}

void CoreSetVideoFunc(Core *ctx, CoreVideoFunc func, void *opaque)
{
	ctx->video_func = func;
	ctx->video_opaque = opaque;
}


// Settings

#define CORE_SETTING_PREFIX "mesen2-"

#define CMP_SETTING(key, suffix) \
	!strcmp(key, CORE_SETTING_PREFIX suffix)

#define CMP_BOOL(b) \
	!strcmp(b, "true")

#define CMP_ENUM(e, val) \
	!strcmp(e, val)

static void core_set_buttons(ControllerConfig *controller, ControllerType type, uint32_t index)
{
	index <<= 8;

	controller->Type = type;

	controller->Keys.Mapping1.A = index | CORE_BUTTON_A;
	controller->Keys.Mapping1.B = index | CORE_BUTTON_B;
	controller->Keys.Mapping1.Start = index | CORE_BUTTON_START;
	controller->Keys.Mapping1.Select = index | CORE_BUTTON_SELECT;
	controller->Keys.Mapping1.Up = index | CORE_BUTTON_DPAD_U;
	controller->Keys.Mapping1.Down = index | CORE_BUTTON_DPAD_D;
	controller->Keys.Mapping1.Left = index | CORE_BUTTON_DPAD_L;
	controller->Keys.Mapping1.Right = index | CORE_BUTTON_DPAD_R;

	if (type == ControllerType::SnesController) {
		controller->Keys.Mapping1.X = index | CORE_BUTTON_X;
		controller->Keys.Mapping1.Y = index | CORE_BUTTON_Y;
		controller->Keys.Mapping1.L = index | CORE_BUTTON_L;
		controller->Keys.Mapping1.R = index | CORE_BUTTON_R;
	}
}

static void core_reset_video_settings(Core *ctx)
{
	VideoConfig cfg = {};

	// Use correct aspect ratio
	cfg.AspectRatio = VideoAspectRatio::Auto;

	ctx->emu->GetSettings()->SetVideoConfig(cfg);
}

static void core_nes_set_stereo(NesConfig &cfg, bool enabled)
{
	if (enabled) {
		// Left
		cfg.ChannelPanning[(int) AudioChannel::Triangle] = -33;
		cfg.ChannelPanning[(int) AudioChannel::Noise] = -33;
		cfg.ChannelPanning[(int) AudioChannel::Square1] = -33;

		// Right
		cfg.ChannelPanning[(int) AudioChannel::DMC] = 33;
		cfg.ChannelPanning[(int) AudioChannel::Square2] = 33;

		// Center
		cfg.ChannelPanning[(int) AudioChannel::FDS] = 0;
		cfg.ChannelPanning[(int) AudioChannel::MMC5] = 0;
		cfg.ChannelPanning[(int) AudioChannel::VRC6] = 0;
		cfg.ChannelPanning[(int) AudioChannel::VRC7] = 0;
		cfg.ChannelPanning[(int) AudioChannel::Namco163] = 0;
		cfg.ChannelPanning[(int) AudioChannel::Sunsoft5B] = 0;

	} else {
		memset(cfg.ChannelPanning, 0, sizeof(cfg.ChannelPanning));
	}
}

static bool core_nes_get_stereo(NesConfig &cfg)
{
	return cfg.ChannelPanning[(int) AudioChannel::Triangle] != 0;
}

static void core_reset_nes_settings(Core *ctx)
{
	NesConfig cfg = {};

	// FDS
	cfg.FdsFastForwardOnLoad = true;
	cfg.FdsAutoInsertDisk = true;

	// Input
	core_set_buttons(&cfg.Port1, ControllerType::NesController, 0);
	core_set_buttons(&cfg.Port2, ControllerType::NesController, 1);

	// Cut overscan top / bottom
	cfg.NtscOverscan.Top = cfg.NtscOverscan.Bottom = 8;

	// Defualt config has volumes all zeroed out
	for (uint32_t x = 0; x < sizeof(cfg.ChannelVolumes) / sizeof(uint32_t); x++)
		cfg.ChannelVolumes[x] = 100;

	// Trianble popping
	cfg.SilenceTriangleHighFreq = true;

	// Fake stereo, same config as merton-nes
	core_nes_set_stereo(cfg, true);

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Default config has zeroed out palette
	memcpy(cfg.UserPalette, PALETTE_NES_DEFAULT, sizeof(PALETTE_NES_DEFAULT));

	// Wait for palette RAM writes before showing frames
	ctx->settings.wait_for_pr = true;

	ctx->emu->GetSettings()->SetNesConfig(cfg);
}

static void core_reset_sms_settings(Core *ctx)
{
	SmsConfig cfg = {};

	// Input
	core_set_buttons(&cfg.Port1, ControllerType::SmsController, 0);
	core_set_buttons(&cfg.Port2, ControllerType::SmsController, 1);

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Defualt config has volumes all zeroed out
	for (uint32_t x = 0; x < sizeof(cfg.ChannelVolumes) / sizeof(uint32_t); x++)
		cfg.ChannelVolumes[x] = 100;

	ctx->emu->GetSettings()->SetSmsConfig(cfg);
}

static void core_reset_pce_settings(Core *ctx)
{
	PcEngineConfig cfg = {};

	// Input
	core_set_buttons(&cfg.Port1, ControllerType::PceController, 0);

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Default config has zeroed out palette
	memcpy(cfg.Palette, PALETTE_PCE_DEFAULT, sizeof(PALETTE_PCE_DEFAULT));

	ctx->emu->GetSettings()->SetPcEngineConfig(cfg);
}

static void core_reset_gameboy_settings(Core *ctx)
{
	GameboyConfig cfg = {};

	// Input
	core_set_buttons(&cfg.Controller, ControllerType::GameboyController, 0);

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Use original gameboy by default
	cfg.Model = GameboyModel::AutoFavorGb;

	ctx->emu->GetSettings()->SetGameboyConfig(cfg);
}

static void core_reset_snes_settings(Core *ctx)
{
	SnesConfig cfg = {};

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Input
	core_set_buttons(&cfg.Port1, ControllerType::SnesController, 0);
	core_set_buttons(&cfg.Port2, ControllerType::SnesController, 1);

	ctx->emu->GetSettings()->SetSnesConfig(cfg);
}

static void core_reset_settings(Core *ctx)
{
	core_reset_video_settings(ctx);
	core_reset_nes_settings(ctx);
	core_reset_sms_settings(ctx);
	core_reset_pce_settings(ctx);
	core_reset_gameboy_settings(ctx);
	core_reset_snes_settings(ctx);
}

void CoreSetSetting(Core *ctx, const char *key, const char *val)
{
	if (!ctx)
		return;

	// NES
	NesConfig &nes_cfg = ctx->emu->GetSettings()->GetNesConfig();

	if (CMP_SETTING(key, "triangle-pop")) {
		nes_cfg.SilenceTriangleHighFreq = !CMP_BOOL(val);

	} else if (CMP_SETTING(key, "stereo")) {
		core_nes_set_stereo(nes_cfg, CMP_BOOL(val));

	} else if (CMP_SETTING(key, "wait-for-pr")) {
		ctx->settings.wait_for_pr = CMP_BOOL(val);
	}
}

const char *CoreGetSetting(Core *ctx, const char *key)
{
	if (!ctx)
		return NULL;

	// NES
	NesConfig &nes_cfg = ctx->emu->GetSettings()->GetNesConfig();

	if (CMP_SETTING(key, "triangle-pop")) {
		return nes_cfg.SilenceTriangleHighFreq ? "false" : "true";

	} else if (CMP_SETTING(key, "stereo")) {
		return core_nes_get_stereo(nes_cfg) ? "true" : "false";

	} else if (CMP_SETTING(key, "wait-for-pr")) {
		return ctx->settings.wait_for_pr ? "true" : "false";
	}

	return NULL;
}

void CoreResetSettings(Core *ctx)
{
	if (!ctx)
		return;

	core_reset_settings(ctx);
}
