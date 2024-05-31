#include "Debugger/Debugger.h"
#include "Debugger/DebugTypes.h"
#include "Debugger/DisassemblyInfo.h"
#include "Debugger/MemoryDumper.h"
#include "Debugger/MemoryAccessCounter.h"
#include "Debugger/CodeDataLogger.h"
#include "Debugger/Disassembler.h"
#include "Debugger/DisassemblySearch.h"
#include "Debugger/BreakpointManager.h"
#include "Debugger/PpuTools.h"
#include "Debugger/DebugBreakHelper.h"
#include "Debugger/LabelManager.h"
#include "Debugger/ScriptManager.h"
#include "Debugger/ScriptHost.h"
#include "Debugger/CallstackManager.h"
#include "Debugger/ExpressionEvaluator.h"
#include "Debugger/BaseEventManager.h"
#include "Debugger/TraceLogFileSaver.h"
#include "Debugger/CdlManager.h"
#include "Debugger/ITraceLogger.h"
#include "Debugger/IDebugger.h"

Debugger::Debugger(Emulator* emu, IConsole* console)
{
}

Debugger::~Debugger()
{
}

template<CpuType type>
void Debugger::ProcessInstruction()
{
}

template<CpuType type, uint8_t accessWidth, MemoryAccessFlags flags, typename T>
void Debugger::ProcessMemoryRead(uint32_t addr, T& value, MemoryOperationType opType)
{
}

template<CpuType type, uint8_t accessWidth, MemoryAccessFlags flags, typename T>
bool Debugger::ProcessMemoryWrite(uint32_t addr, T& value, MemoryOperationType opType)
{
	return false;
}

template<CpuType cpuType, MemoryType memType, MemoryOperationType opType, typename T>
void Debugger::ProcessMemoryAccess(uint32_t addr, T& value)
{
}

template<CpuType type>
void Debugger::ProcessIdleCycle()
{
}

template<CpuType type>
void Debugger::ProcessHaltedCpu()
{
}

template<CpuType type, typename T>
void Debugger::ProcessPpuRead(uint16_t addr, T& value, MemoryType memoryType, MemoryOperationType opType)
{
}

template<CpuType type, typename T>
void Debugger::ProcessPpuWrite(uint16_t addr, T& value, MemoryType memoryType)
{
}

template<CpuType type>
void Debugger::ProcessPpuCycle()
{
}

template<CpuType type>
void Debugger::ProcessInterrupt(uint32_t originalPc, uint32_t currentPc, bool forNmi)
{
}

void Debugger::ProcessConfigChange()
{
}

void Debugger::Step(CpuType cpuType, int32_t stepCount, StepType type, BreakSource source)
{
}

bool Debugger::IsPaused()
{
	return false;
}

bool Debugger::IsExecutionStopped()
{
	return false;
}

bool Debugger::HasBreakRequest()
{
	return false;
}

void Debugger::BreakRequest(bool release)
{
}

void Debugger::ResetSuspendCounter()
{
}

DebuggerFeatures Debugger::GetDebuggerFeatures(CpuType cpuType)
{
	return {};
}

void Debugger::Log(string message)
{
}

PpuTools* Debugger::GetPpuTools(CpuType cpuType)
{
	return nullptr;
}

template void Debugger::ProcessInstruction<CpuType::Snes>();
template void Debugger::ProcessInstruction<CpuType::Sa1>();
template void Debugger::ProcessInstruction<CpuType::Spc>();
template void Debugger::ProcessInstruction<CpuType::Gsu>();
template void Debugger::ProcessInstruction<CpuType::NecDsp>();
template void Debugger::ProcessInstruction<CpuType::Cx4>();
template void Debugger::ProcessInstruction<CpuType::Gameboy>();
template void Debugger::ProcessInstruction<CpuType::Nes>();
template void Debugger::ProcessInstruction<CpuType::Pce>();
template void Debugger::ProcessInstruction<CpuType::Sms>();
template void Debugger::ProcessInstruction<CpuType::Gba>();

template void Debugger::ProcessMemoryRead<CpuType::Snes>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Sa1>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Spc>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Spc, 1, MemoryAccessFlags::DspAccess>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Gsu>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::NecDsp>(uint32_t addr, uint32_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::NecDsp>(uint32_t addr, uint16_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Cx4>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Gameboy>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Nes>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Pce>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Sms>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Gba, 1>(uint32_t addr, uint32_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Gba, 2>(uint32_t addr, uint32_t& value, MemoryOperationType opType);
template void Debugger::ProcessMemoryRead<CpuType::Gba, 4>(uint32_t addr, uint32_t& value, MemoryOperationType opType);

template bool Debugger::ProcessMemoryWrite<CpuType::Snes>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Sa1>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Spc>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Spc, 1, MemoryAccessFlags::DspAccess>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Gsu>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::NecDsp>(uint32_t addr, uint16_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Cx4>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Gameboy>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Nes>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Pce>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Sms>(uint32_t addr, uint8_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Gba, 1>(uint32_t addr, uint32_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Gba, 2>(uint32_t addr, uint32_t& value, MemoryOperationType opType);
template bool Debugger::ProcessMemoryWrite<CpuType::Gba, 4>(uint32_t addr, uint32_t& value, MemoryOperationType opType);

template void Debugger::ProcessMemoryAccess<CpuType::Pce, MemoryType::PceAdpcmRam, MemoryOperationType::Write>(uint32_t addr, uint8_t& value);
template void Debugger::ProcessMemoryAccess<CpuType::Pce, MemoryType::PceAdpcmRam, MemoryOperationType::Read>(uint32_t addr, uint8_t& value);
template void Debugger::ProcessMemoryAccess<CpuType::Pce, MemoryType::PceArcadeCardRam, MemoryOperationType::Write>(uint32_t addr, uint8_t& value);
template void Debugger::ProcessMemoryAccess<CpuType::Pce, MemoryType::PceArcadeCardRam, MemoryOperationType::Read>(uint32_t addr, uint8_t& value);
template void Debugger::ProcessMemoryAccess<CpuType::Sms, MemoryType::SmsPort, MemoryOperationType::Write>(uint32_t addr, uint8_t& value);
template void Debugger::ProcessMemoryAccess<CpuType::Sms, MemoryType::SmsPort, MemoryOperationType::Read>(uint32_t addr, uint8_t& value);

template void Debugger::ProcessIdleCycle<CpuType::Snes>();
template void Debugger::ProcessIdleCycle<CpuType::Sa1>();

template void Debugger::ProcessHaltedCpu<CpuType::Snes>();
template void Debugger::ProcessHaltedCpu<CpuType::Spc>();
template void Debugger::ProcessHaltedCpu<CpuType::Gameboy>();
template void Debugger::ProcessHaltedCpu<CpuType::Sms>();
template void Debugger::ProcessHaltedCpu<CpuType::Gba>();

template void Debugger::ProcessInterrupt<CpuType::Snes>(uint32_t originalPc, uint32_t currentPc, bool forNmi);
template void Debugger::ProcessInterrupt<CpuType::Sa1>(uint32_t originalPc, uint32_t currentPc, bool forNmi);
template void Debugger::ProcessInterrupt<CpuType::Gameboy>(uint32_t originalPc, uint32_t currentPc, bool forNmi);
template void Debugger::ProcessInterrupt<CpuType::Nes>(uint32_t originalPc, uint32_t currentPc, bool forNmi);
template void Debugger::ProcessInterrupt<CpuType::Pce>(uint32_t originalPc, uint32_t currentPc, bool forNmi);
template void Debugger::ProcessInterrupt<CpuType::Sms>(uint32_t originalPc, uint32_t currentPc, bool forNmi);
template void Debugger::ProcessInterrupt<CpuType::Gba>(uint32_t originalPc, uint32_t currentPc, bool forNmi);

template void Debugger::ProcessPpuRead<CpuType::Snes>(uint16_t addr, uint8_t& value, MemoryType memoryType, MemoryOperationType opType);
template void Debugger::ProcessPpuRead<CpuType::Gameboy>(uint16_t addr, uint8_t& value, MemoryType memoryType, MemoryOperationType opType);
template void Debugger::ProcessPpuRead<CpuType::Nes>(uint16_t addr, uint8_t& value, MemoryType memoryType, MemoryOperationType opType);
template void Debugger::ProcessPpuRead<CpuType::Pce>(uint16_t addr, uint16_t& value, MemoryType memoryType, MemoryOperationType opType);
template void Debugger::ProcessPpuRead<CpuType::Pce>(uint16_t addr, uint8_t& value, MemoryType memoryType, MemoryOperationType opType);
template void Debugger::ProcessPpuRead<CpuType::Sms>(uint16_t addr, uint8_t& value, MemoryType memoryType, MemoryOperationType opType);

template void Debugger::ProcessPpuWrite<CpuType::Snes>(uint16_t addr, uint8_t& value, MemoryType memoryType);
template void Debugger::ProcessPpuWrite<CpuType::Gameboy>(uint16_t addr, uint8_t& value, MemoryType memoryType);
template void Debugger::ProcessPpuWrite<CpuType::Nes>(uint16_t addr, uint8_t& value, MemoryType memoryType);
template void Debugger::ProcessPpuWrite<CpuType::Pce>(uint16_t addr, uint16_t& value, MemoryType memoryType);
template void Debugger::ProcessPpuWrite<CpuType::Pce>(uint16_t addr, uint8_t& value, MemoryType memoryType);
template void Debugger::ProcessPpuWrite<CpuType::Sms>(uint16_t addr, uint8_t& value, MemoryType memoryType);

template void Debugger::ProcessPpuCycle<CpuType::Snes>();
template void Debugger::ProcessPpuCycle<CpuType::Gameboy>();
template void Debugger::ProcessPpuCycle<CpuType::Nes>();
template void Debugger::ProcessPpuCycle<CpuType::Pce>();
template void Debugger::ProcessPpuCycle<CpuType::Sms>();
template void Debugger::ProcessPpuCycle<CpuType::Gba>();
