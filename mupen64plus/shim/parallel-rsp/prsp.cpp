#include "prsp.h"

#include "device/rcp/rsp/rsp_core.h"

#include "rsp_jit.hpp"

namespace RSP {
	RSP::JIT::CPU cpu;
	RSP_INFO rsp;
	int16_t MFC0_count[32];
	int32_t SP_STATUS_TIMEOUT;
}

extern "C" void core_log(const char *fmt, ...);

m64p_error PRSP_PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion,
	int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
	if (PluginType)
		*PluginType = M64PLUGIN_RSP;

	if (PluginVersion)
		*PluginVersion = 0x010000;

	if (APIVersion)
		*APIVersion = 0x020000;

	if (PluginNamePtr)
		*PluginNamePtr = "parallel-rsp";

	if (Capabilities)
		*Capabilities = 0;

	return M64ERR_SUCCESS;
}

unsigned int PRSP_DoRspCycles(unsigned int cycles)
{
	if (*RSP::rsp.SP_STATUS_REG & SP_STATUS_HALT)
		return 0;

	RSP::cpu.invalidate_imem();
	RSP::cpu.get_state().pc = *RSP::rsp.SP_PC_REG & 0xFFF;

	for (uint8_t x = 0; x < 32; x++)
		RSP::MFC0_count[x] = 0;

	while (!(*RSP::rsp.SP_STATUS_REG & SP_STATUS_HALT)) {
		RSP::ReturnMode mode = RSP::cpu.run();

		if (mode == RSP::MODE_CHECK_FLAGS && (*RSP::cpu.get_state().cp0.irq & 1))
			break;
	}

	*RSP::rsp.SP_PC_REG = 0x4001000 | (RSP::cpu.get_state().pc & 0xFFC);

	if (*RSP::rsp.SP_STATUS_REG & SP_STATUS_BROKE) {
		return cycles;

	} else if (*RSP::cpu.get_state().cp0.irq & 1) {
		RSP::rsp.CheckInterrupts();

	} else if (*RSP::rsp.SP_SEMAPHORE_REG != 0) {
	} else {
		RSP::SP_STATUS_TIMEOUT = 16;
	}

	*RSP::rsp.SP_STATUS_REG &= ~SP_STATUS_HALT;

	return cycles;
}

void PRSP_InitiateRSP(RSP_INFO Rsp_Info, unsigned int *CycleCount)
{
	if (CycleCount)
		*CycleCount = 0;

	if (Rsp_Info.DMEM == Rsp_Info.IMEM)
		return;

	RSP::rsp = Rsp_Info;
	*RSP::rsp.SP_PC_REG = 0x4001000 & 0xFFF;

	uint32_t **cr = RSP::cpu.get_state().cp0.cr;
	cr[0x0] = RSP::rsp.SP_MEM_ADDR_REG;
	cr[0x1] = RSP::rsp.SP_DRAM_ADDR_REG;
	cr[0x2] = RSP::rsp.SP_RD_LEN_REG;
	cr[0x3] = RSP::rsp.SP_WR_LEN_REG;
	cr[0x4] = RSP::rsp.SP_STATUS_REG;
	cr[0x5] = RSP::rsp.SP_DMA_FULL_REG;
	cr[0x6] = RSP::rsp.SP_DMA_BUSY_REG;
	cr[0x7] = RSP::rsp.SP_SEMAPHORE_REG;
	cr[0x8] = RSP::rsp.DPC_START_REG;
	cr[0x9] = RSP::rsp.DPC_END_REG;
	cr[0xA] = RSP::rsp.DPC_CURRENT_REG;
	cr[0xB] = RSP::rsp.DPC_STATUS_REG;
	cr[0xC] = RSP::rsp.DPC_CLOCK_REG;
	cr[0xD] = RSP::rsp.DPC_BUFBUSY_REG;
	cr[0xE] = RSP::rsp.DPC_PIPEBUSY_REG;
	cr[0xF] = RSP::rsp.DPC_TMEM_REG;

	*cr[RSP::CP0_REGISTER_SP_STATUS] = SP_STATUS_HALT;
	RSP::cpu.get_state().cp0.irq = RSP::rsp.MI_INTR_REG;

	RSP::SP_STATUS_TIMEOUT = 0x7FFF;

	RSP::cpu.set_dmem((uint32_t *) Rsp_Info.DMEM);
	RSP::cpu.set_imem((uint32_t *) Rsp_Info.IMEM);
	RSP::cpu.set_rdram((uint32_t *) Rsp_Info.RDRAM);
}

void PRSP_RomClosed(void)
{
	*RSP::rsp.SP_PC_REG = 0;
}
