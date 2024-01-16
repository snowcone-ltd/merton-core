#define PluginStartup RDP_PluginStartup
#define PluginShutdown RDP_PluginShutdown
#define PluginGetVersion RDP_PluginGetVersion
#define InitiateGFX RDP_InitiateGFX
#define MoveScreen RDP_MoveScreen
#define ProcessDList RDP_ProcessDList
#define ProcessRDPList RDP_ProcessRDPList
#define RomOpen RDP_RomOpen
#define RomClosed RDP_RomClosed
#define ShowCFB RDP_ShowCFB
#define UpdateScreen RDP_UpdateScreen
#define ViStatusChanged RDP_ViStatusChanged
#define ViWidthChanged RDP_ViWidthChanged
#define ChangeWindow RDP_ChangeWindow
#define ReadScreen2 RDP_ReadScreen2
#define SetRenderingCallback RDP_SetRenderingCallback
#define ResizeVideoOutput RDP_ResizeVideoOutput
#define FBWrite RDP_FBWrite
#define FBRead RDP_FBRead
#define FBGetFrameBufferInfo RDP_FBGetFrameBufferInfo

#include "../../../angrylion-rdp-plus/src/plugin/mupen64plus/gfx_m64p.c"
