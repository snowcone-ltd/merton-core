#pragma once

#include "api/m64p_plugin.h"
#include "api/m64p_types.h"

#if defined(_MSC_VER)
	#define PRDP_EXPORT __declspec(dllexport)
#else
	#define PRDP_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

void prdp_set_func(void (*func)(void *, uint32_t, uint32_t, void *), void *opaque);

PRDP_EXPORT m64p_error PRDP_PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion,
	int *APIVersion, const char **PluginNamePtr, int *Capabilities);
PRDP_EXPORT int PRDP_InitiateGFX(GFX_INFO Gfx_Info);
PRDP_EXPORT void PRDP_MoveScreen(int xpos, int ypos);
PRDP_EXPORT void PRDP_ProcessDList(void);
PRDP_EXPORT void PRDP_ProcessRDPList(void);
PRDP_EXPORT int PRDP_RomOpen(void);
PRDP_EXPORT void PRDP_RomClosed(void);
PRDP_EXPORT void PRDP_ShowCFB(void);
PRDP_EXPORT void PRDP_UpdateScreen(void);
PRDP_EXPORT void PRDP_ViStatusChanged(void);
PRDP_EXPORT void PRDP_ViWidthChanged(void);
PRDP_EXPORT void PRDP_ChangeWindow(void);
PRDP_EXPORT void PRDP_ReadScreen2(void *dest, int *width, int *height, int front);
PRDP_EXPORT void PRDP_SetRenderingCallback(void (*callback)(int));
PRDP_EXPORT void PRDP_ResizeVideoOutput(int width, int height);
PRDP_EXPORT void PRDP_FBWrite(unsigned int addr, unsigned int size);
PRDP_EXPORT void PRDP_FBRead(unsigned int addr);
PRDP_EXPORT void PRDP_FBGetFrameBufferInfo(void *pinfo);

#ifdef __cplusplus
}
#endif
