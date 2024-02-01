#pragma once

#include "api/m64p_plugin.h"
#include "api/m64p_types.h"

#ifdef __cplusplus
extern "C" {
#endif

m64p_error PRDP_PluginStartup(m64p_dynlib_handle _CoreLibHandle, void *Context,
	void (*DebugCallback)(void *, int, const char *));
m64p_error PRDP_PluginShutdown(void);
m64p_error PRDP_PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion,
	int *APIVersion, const char **PluginNamePtr, int *Capabilities);
int PRDP_InitiateGFX(GFX_INFO Gfx_Info);
void PRDP_MoveScreen(int xpos, int ypos);
void PRDP_ProcessDList(void);
void PRDP_ProcessRDPList(void);
int PRDP_RomOpen(void);
void PRDP_RomClosed(void);
void PRDP_ShowCFB(void);
void PRDP_UpdateScreen(void);
void PRDP_ViStatusChanged(void);
void PRDP_ViWidthChanged(void);
void PRDP_ChangeWindow(void);
void PRDP_ReadScreen2(void *dest, int *width, int *height, int front);
void PRDP_SetRenderingCallback(void (*callback)(int));
void PRDP_ResizeVideoOutput(int width, int height);
void PRDP_FBWrite(unsigned int addr, unsigned int size);
void PRDP_FBRead(unsigned int addr);
void PRDP_FBGetFrameBufferInfo(void *pinfo);

#ifdef __cplusplus
}
#endif
