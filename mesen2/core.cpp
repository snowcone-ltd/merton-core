#include "core.h"

#include <stdlib.h>
#include <stdarg.h>

#include "Core/NES/GameDatabase.h"
#include "Core/NES/NesConsole.h"
#include "Core/NES/APU/NesApu.h"
#include "Core/Shared/Emulator.h"
#include "Core/Shared/EmuSettings.h"
#include "Core/Shared/KeyManager.h"
#include "Core/Shared/Video/BaseVideoFilter.h"
#include "Core/Shared/MessageManager.h"
#include "Core/Shared/SaveStateManager.h"
#include "Utilities/VirtualFile.h"
#include "Utilities/FolderUtilities.h"
#include "Utilities/miniz.h"
#include "Utilities/Serializer.h"

#include "shim/CoreKeyManager.h"
#include "settings.h"
#include "palette.h"
#include "romdb.h"

#if defined(_MSC_VER)
	#define strdup _strdup
#endif

struct Core {
	Emulator *emu;
	BaseVideoFilter *filter;
	CoreKeyManager *km;
	uint8_t pr_initial[0x20];
	bool pr_written;

	struct {
		bool wait_for_pr;
	} settings;
};

static CoreLogFunc CORE_LOG;
static CoreVideoFunc CORE_VIDEO;
static void *CORE_LOG_OPAQUE;
static void *CORE_VIDEO_OPAQUE;


// Shimmed

void battery_manager_set_save_data(const void *save_data, size_t save_data_size);
const void *battery_manager_get_save_data(size_t *save_data_size);
void emulator_set_system(CoreSystem system);
void sound_mixer_set_func(CoreAudioFunc func, void *opaque);


// API

void core_log(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char msg[1024];
	vsnprintf(msg, 1024, fmt, arg);

	CORE_LOG(msg, CORE_LOG_OPAQUE);

	va_end(arg);
}

static void core_load_rom_db(void)
{
	uLongf usize = MESEN_DB_USIZE;
	unsigned char *ubuf = (unsigned char *) malloc(usize);

	if (uncompress(ubuf, &usize, MESEN_DB, sizeof(MESEN_DB)) == Z_OK) {
		std::stringstream db;
		db.write((const char *) ubuf, usize);
		GameDatabase::LoadGameDb(db);
		MessageManager::ClearLog();
	}

	free(ubuf);
}

static void core_set_buttons(ControllerConfig *controller, ControllerType type, uint32_t index)
{
	index <<= 8;

	controller->Type = type;

	// All Mesen mappings use the Nintendo layout

	controller->Keys.Mapping1.A = index | CORE_BUTTON_B;
	controller->Keys.Mapping1.B = index | CORE_BUTTON_A;
	controller->Keys.Mapping1.Start = index | CORE_BUTTON_START;
	controller->Keys.Mapping1.Select = index | CORE_BUTTON_SELECT;
	controller->Keys.Mapping1.Up = index | CORE_BUTTON_DPAD_U;
	controller->Keys.Mapping1.Down = index | CORE_BUTTON_DPAD_D;
	controller->Keys.Mapping1.Left = index | CORE_BUTTON_DPAD_L;
	controller->Keys.Mapping1.Right = index | CORE_BUTTON_DPAD_R;

	if (type == ControllerType::SnesController || type == ControllerType::GbaController) {
		controller->Keys.Mapping1.L = index | CORE_BUTTON_L;
		controller->Keys.Mapping1.R = index | CORE_BUTTON_R;
	}

	if (type == ControllerType::SnesController) {
		controller->Keys.Mapping1.X = index | CORE_BUTTON_Y;
		controller->Keys.Mapping1.Y = index | CORE_BUTTON_X;
	}
}

static void core_set_video_settings(Core *ctx)
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

static void core_set_nes_settings(Core *ctx)
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
	cfg.SilenceTriangleHighFreq = !CMP_BOOL("triangle-pop");

	// Fake stereo, same config as merton-nes
	core_nes_set_stereo(cfg, CMP_BOOL("stereo"));

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Default config has zeroed out palette
	memcpy(cfg.UserPalette, PALETTE_NES_DEFAULT, sizeof(PALETTE_NES_DEFAULT));

	// Wait for palette RAM writes before showing frames
	ctx->settings.wait_for_pr = CMP_BOOL("wait-for-pr");

	ctx->emu->GetSettings()->SetNesConfig(cfg);
}

static void core_set_sms_settings(Core *ctx)
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

static void core_set_pce_settings(Core *ctx)
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

static void core_set_gameboy_settings(Core *ctx)
{
	GameboyConfig cfg = {};

	// Input
	core_set_buttons(&cfg.Controller, ControllerType::GameboyController, 0);

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Do not blur frames
	cfg.BlendFrames = false;

	// Original vs. color
	cfg.Model = CMP_ENUM("model", "Game Boy") ? GameboyModel::Gameboy : GameboyModel::GameboyColor;

	ctx->emu->GetSettings()->SetGameboyConfig(cfg);
}

static void core_set_snes_settings(Core *ctx)
{
	SnesConfig cfg = {};

	// Zero out RAM
	cfg.RamPowerOnState = RamState::AllZeros;

	// Input
	core_set_buttons(&cfg.Port1, ControllerType::SnesController, 0);
	core_set_buttons(&cfg.Port2, ControllerType::SnesController, 1);

	ctx->emu->GetSettings()->SetSnesConfig(cfg);
}

static void core_set_gba_settings(Core *ctx)
{
	GbaConfig cfg = {};

	// Do not blur frames
	cfg.BlendFrames = false;

	// Input
	core_set_buttons(&cfg.Controller, ControllerType::GbaController, 0);

	ctx->emu->GetSettings()->SetGbaConfig(cfg);
}

static void core_set_settings(Core *ctx)
{
	core_set_video_settings(ctx);
	core_set_nes_settings(ctx);
	core_set_sms_settings(ctx);
	core_set_pce_settings(ctx);
	core_set_gameboy_settings(ctx);
	core_set_snes_settings(ctx);
	core_set_gba_settings(ctx);
}

void CoreUnloadGame(Core **core)
{
	if (!core || !*core)
		return;

	Core *ctx = *core;

	ctx->pr_written = false;

	battery_manager_set_save_data(NULL, 0);
	sound_mixer_set_func(NULL, NULL);

	if (ctx->emu)
		ctx->emu->Stop(false);

	if (ctx->km)
		delete ctx->km;

	if (ctx->filter)
		delete ctx->filter;

	if (ctx->emu)
		delete ctx->emu;

	free(ctx);
	*core = NULL;
}

static void core_get_logs(void)
{
	string log = MessageManager::GetLog();
	if (log.empty())
		return;

	CORE_LOG(log.c_str(), CORE_LOG_OPAQUE);
	MessageManager::ClearLog();
}

static void core_nes_init_triangle(Core *ctx)
{
	std::stringstream ss;

	Serializer s_in(SaveStateManager::FileFormatVersion, true);

	uint8_t val = 15;
	s_in.Stream(val, "triangle.sequencePosition");
	s_in.SaveTo(ss, 0);

	Serializer s_out(SaveStateManager::FileFormatVersion, false);
	s_out.LoadFrom(ss);

	NesConsole *nes = (NesConsole *) ctx->emu->GetConsoleUnsafe();
	NesApu *apu = nes->GetApu();
	apu->Serialize(s_out);
}

Core *CoreLoadGame(CoreSystem system, const char *systemDir, const char *path,
	const void *saveData, size_t saveDataSize)
{
	Core *ctx = (Core *) calloc(1, sizeof(Core));

	core_load_rom_db();
	FolderUtilities::SetFolderOverrides("", "", "", systemDir);

	ctx->emu = new Emulator();
	emulator_set_system(system);
	core_set_settings(ctx);

	KeyManager::SetSettings(ctx->emu->GetSettings());

	ctx->km = new CoreKeyManager(ctx->emu);
	KeyManager::RegisterKeyManager(ctx->km);

	battery_manager_set_save_data(saveData, saveDataSize);

	VirtualFile vfile = VirtualFile(path);

	bool loaded = ctx->emu->LoadRom(vfile, {});

	if (loaded) {
		ctx->filter = ctx->emu->GetVideoFilter(true);

		if (system == CORE_SYSTEM_NES) {
			core_nes_init_triangle(ctx);

			ConsoleMemoryInfo mi = ctx->emu->GetMemory(MemoryType::NesPaletteRam);
			memcpy(ctx->pr_initial, mi.Memory, 0x20);

		} else {
			ctx->pr_written = true;
		}
	} else {
		CoreUnloadGame(&ctx);
	}

	core_get_logs();

	return ctx;
}

void CoreReset(Core *ctx)
{
	if (!ctx)
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

static void core_get_frame(Core *ctx)
{
	if (!core_palette_ram_written(ctx)) {
		CORE_VIDEO(NULL, CORE_COLOR_FORMAT_UNKNOWN, 0, 0, 0, CORE_VIDEO_OPAQUE);
		return;
	}

	PpuFrameInfo pi = ctx->emu->GetPpuFrame();
	ctx->filter->SetBaseFrameInfo({pi.Width, pi.Height});
	FrameInfo fi = ctx->filter->SendFrame((uint16_t *) pi.FrameBuffer, 0, 0, nullptr, true);

	uint32_t *frame = ctx->filter->GetOutputBuffer();

	CORE_VIDEO(frame, CORE_COLOR_FORMAT_BGRA, fi.Width, fi.Height, fi.Width * 4, CORE_VIDEO_OPAQUE);
}

void CoreRun(Core *ctx)
{
	if (!ctx)
		return;

	ctx->emu->Run();

	core_get_frame(ctx);
	core_get_logs();
}

void CorePauseThreads(Core *ctx, bool pause)
{
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx)
		return;

	ctx->km->SetButton(player, button, pressed);
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx)
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
	if (!ctx)
		return false;

	std::stringstream ss;
	ss.write((char *) state, size);

	return ctx->emu->Deserialize(ss, 0, false, std::optional<ConsoleType>()) ==
		DeserializeResult::Success;
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	// Mesen auto loads FDS disks

	return false;
}

void *CoreGetSaveData(Core *ctx, size_t *size, CoreSaveDataType *type)
{
	if (!ctx)
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

double CoreGetFrameRate(Core *ctx)
{
	if (!ctx)
		return 60;

	return ctx->emu->GetFps();
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx)
		return 1;

	PpuFrameInfo fi = ctx->emu->GetPpuFrame();

	return ctx->emu->GetSettings()->GetAspectRatio(ctx->emu->GetRegion(), {fi.Width, fi.Height});
}

void CoreSetLogFunc(CoreLogFunc func, void *opaque)
{
	CORE_LOG = func;
	CORE_LOG_OPAQUE = opaque;
}

void CoreSetAudioFunc(CoreAudioFunc func, void *opaque)
{
	sound_mixer_set_func(func, opaque);
}

void CoreSetVideoFunc(CoreVideoFunc func, void *opaque)
{
	CORE_VIDEO = func;
	CORE_VIDEO_OPAQUE = opaque;
}

CoreSetting *CoreGetSettings(uint32_t *len)
{
	*len = sizeof(CORE_SETTINGS) / sizeof(CoreSetting);

	return CORE_SETTINGS;
}

void CoreUpdateSettings(Core *ctx)
{
	if (!ctx)
		return;

	core_set_settings(ctx);
}
