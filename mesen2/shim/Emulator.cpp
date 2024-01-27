#include "Shared/Emulator.h"

#include "Shared/NotificationManager.h"
#include "Shared/Audio/SoundMixer.h"
#include "Shared/Audio/AudioPlayerHud.h"
#include "Shared/Video/VideoDecoder.h"
#include "Shared/Video/VideoRenderer.h"
#include "Shared/Video/DebugHud.h"
#include "Shared/FrameLimiter.h"
#include "Shared/MessageManager.h"
#include "Shared/KeyManager.h"
#include "Shared/EmuSettings.h"
#include "Shared/SaveStateManager.h"
#include "Shared/Video/DebugStats.h"
#include "Shared/RewindManager.h"
#include "Shared/EmulatorLock.h"
#include "Shared/DebuggerRequest.h"
#include "Shared/Movies/MovieManager.h"
#include "Shared/BatteryManager.h"
#include "Shared/CheatManager.h"
#include "Shared/SystemActionManager.h"
#include "Shared/Movies/MovieManager.h"
#include "Shared/TimingInfo.h"
#include "Shared/HistoryViewer.h"
#include "Shared/BaseControlManager.h"
#include "Netplay/GameServer.h"
#include "Netplay/GameClient.h"
#include "Utilities/Serializer.h"

#include "NES/NesConsole.h"
#include "NES/NesTypes.h"
#include "SMS/SmsConsole.h"
#include "PCE/PceConsole.h"
#include "Gameboy/Gameboy.h"
#include "SNES/SnesConsole.h"

#include "core.h"

static CoreSystem CORE_SYSTEM = CORE_SYSTEM_UNKNOWN;

Emulator::Emulator() :
	_settings(new EmuSettings(this)),
	_debugHud(new DebugHud()),
	_scriptHud(new DebugHud()),
	_notificationManager(new NotificationManager()),
	_batteryManager(new BatteryManager()),
	_soundMixer(new SoundMixer(this)), // Shimmed
	_videoRenderer(new VideoRenderer(this)), // Shimmed
	_videoDecoder(new VideoDecoder(this)), // Shimmed
	_saveStateManager(new SaveStateManager(this)),
	_cheatManager(new CheatManager(this)),
	_movieManager(new MovieManager(this)),
	_gameServer(new GameServer(this)),
	_gameClient(new GameClient(this)),
	_rewindManager(new RewindManager(this))
{
	// XXX If this is not initialized BaseControlManager will crash
	_systemActionManager.reset(new SystemActionManager(this));
}

Emulator::~Emulator()
{
}

void Emulator::Run()
{
	_console->RunFrame();
}

void Emulator::OnBeforeSendFrame()
{
}

void Emulator::ProcessEndOfFrame()
{
}

void Emulator::Stop(bool sendNotification, bool preventRecentGameSave, bool saveBattery)
{
	if (_console)
		_console.reset();
}

void Emulator::Reset()
{
	if (_console)
		_console->Reset();
}

void Emulator::ReloadRom(bool forPowerCycle)
{
}

void Emulator::PowerCycle()
{
}

bool Emulator::LoadRom(VirtualFile romFile, VirtualFile patchFile, bool stopRom, bool forPowerCycle)
{
	if (!romFile.IsValid())
		return false;

	switch (CORE_SYSTEM) {
		case CORE_SYSTEM_NES:     _console.reset(new NesConsole(this));  break;
		case CORE_SYSTEM_SMS:     _console.reset(new SmsConsole(this));  break;
		case CORE_SYSTEM_TG16:    _console.reset(new PceConsole(this));  break;
		case CORE_SYSTEM_GAMEBOY: _console.reset(new Gameboy(this));     break;
		case CORE_SYSTEM_SNES:    _console.reset(new SnesConsole(this)); break;
		default:
			break;
	}

	_consoleType = _console->GetConsoleType();

	LoadRomResult r = _console->LoadRom(romFile);

	if (r == LoadRomResult::Success) {
		_rom.RomFile = (string) romFile;
		_rom.PatchFile = (string) patchFile;
		_rom.Format = _console->GetRomFormat();
		_rom.DipSwitches = _console->GetDipSwitchInfo();

		return true;
	}

	return false;
}

string Emulator::GetHash(HashType type)
{
	string hash = _console->GetHash(type);

	if (hash.size()) {
		return hash;

	} else if (type == HashType::Sha1) {
		return _rom.RomFile.GetSha1Hash();

	} else if (type == HashType::Sha1Cheat) {
		return _rom.RomFile.GetSha1Hash();
	}

	return "";
}

uint32_t Emulator::GetCrc32()
{
	return _rom.RomFile.GetCrc32();
}

PpuFrameInfo Emulator::GetPpuFrame()
{
	return _console->GetPpuFrame();
}

ConsoleRegion Emulator::GetRegion()
{
	return _console->GetRegion();
}

shared_ptr<IConsole> Emulator::GetConsole()
{
	return _console.lock();
}

IConsole* Emulator::GetConsoleUnsafe()
{
	return _console.get();
}

ConsoleType Emulator::GetConsoleType()
{
	return _consoleType;
}

uint64_t Emulator::GetMasterClock()
{
	return _console->GetMasterClock();
}

uint32_t Emulator::GetMasterClockRate()
{
	return _console->GetMasterClockRate();
}

uint32_t Emulator::GetFrameCount()
{
	return GetPpuFrame().FrameCount;
}

uint32_t Emulator::GetLagCounter()
{
	return _console->GetControlManager()->GetLagCounter();
}

void Emulator::RegisterInputRecorder(IInputRecorder* recorder)
{
	if (_console)
		_console->GetControlManager()->RegisterInputRecorder(recorder);
}

void Emulator::UnregisterInputRecorder(IInputRecorder* recorder)
{
	if (_console)
		_console->GetControlManager()->UnregisterInputRecorder(recorder);
}

void Emulator::RegisterInputProvider(IInputProvider* provider)
{
	if (_console)
		_console->GetControlManager()->RegisterInputProvider(provider);
}

void Emulator::UnregisterInputProvider(IInputProvider* provider)
{
	if (_console)
		_console->GetControlManager()->UnregisterInputProvider(provider);
}

double Emulator::GetFps()
{
	return _console->GetFps();
}

double Emulator::GetFrameDelay()
{
	return 0;
}

void Emulator::PauseOnNextFrame()
{
}

void Emulator::Pause()
{
}

void Emulator::Resume()
{
}

bool Emulator::IsPaused()
{
	return false;
}

EmulatorLock Emulator::AcquireLock(bool allowDebuggerLock)
{
	return EmulatorLock(this, false);
}

void Emulator::Lock()
{
}

void Emulator::Unlock()
{
}

void Emulator::SuspendDebugger(bool release)
{
}

void Emulator::Serialize(ostream& out, bool includeSettings, int compressionLevel)
{
	Serializer s(SaveStateManager::FileFormatVersion, true);

	s.Stream(_console, "");
	s.SaveTo(out, 0);
}

bool Emulator::Deserialize(istream& in, uint32_t fileFormatVersion, bool includeSettings, optional<ConsoleType> consoleType, bool sendNotification)
{
	Serializer s(SaveStateManager::FileFormatVersion, false);

	if (!s.LoadFrom(in))
		return false;

	s.Stream(_console, "");

	return true;
}

BaseVideoFilter* Emulator::GetVideoFilter(bool getDefaultFilter)
{
	return _console->GetVideoFilter(getDefaultFilter);
}

void Emulator::BlockDebuggerRequests()
{
}

DebuggerRequest Emulator::GetDebugger(bool autoInit)
{
	return DebuggerRequest(nullptr);
}

bool Emulator::IsEmulationThread()
{
	return true;
}

void Emulator::RegisterMemory(MemoryType type, void* memory, uint32_t size)
{
	_consoleMemory[(int) type] = {memory, size};
}

ConsoleMemoryInfo Emulator::GetMemory(MemoryType type)
{
	return _consoleMemory[(int) type];
}

AudioTrackInfo Emulator::GetAudioTrackInfo()
{
	return {};
}

void Emulator::ProcessEvent(EventType type, std::optional<CpuType> cpuType)
{
}

template<CpuType cpuType>
void Emulator::AddDebugEvent(DebugEventType evtType)
{
}

void Emulator::BreakIfDebugging(CpuType sourceCpu, BreakSource source)
{
}

template void Emulator::AddDebugEvent<CpuType::Snes>(DebugEventType evtType);
template void Emulator::AddDebugEvent<CpuType::Gameboy>(DebugEventType evtType);
template void Emulator::AddDebugEvent<CpuType::Nes>(DebugEventType evtType);
template void Emulator::AddDebugEvent<CpuType::Pce>(DebugEventType evtType);

void emulator_set_system(CoreSystem system)
{
	CORE_SYSTEM = system;
}
