#pragma once

#include "api/m64p_plugin.h"
#include "api/m64p_types.h"

#if defined(_MSC_VER)
	#define PRSP_EXPORT __declspec(dllexport)
#else
	#define PRSP_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

PRSP_EXPORT m64p_error PRSP_PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion,
	int *APIVersion, const char **PluginNamePtr, int *Capabilities);
PRSP_EXPORT unsigned int PRSP_DoRspCycles(unsigned int cycles);
PRSP_EXPORT void PRSP_InitiateRSP(RSP_INFO Rsp_Info, unsigned int *CycleCount);
PRSP_EXPORT void PRSP_RomClosed(void);

#ifdef __cplusplus
}
#endif
