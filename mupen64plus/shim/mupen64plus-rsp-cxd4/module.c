#define PluginStartup RSP_PluginStartup
#define PluginShutdown RSP_PluginShutdown
#define PluginGetVersion RSP_PluginGetVersion
#define RomOpen RSP_RomOpen
#define CloseDLL RSP_CloseDLL
#define DllAbout RSP_DllAbout
#define DllConfig RSP_DllConfig
#define DoRspCycles RSP_DoRspCycles
#define GetDllInfo RSP_GetDllInfo
#define InitiateRSP RSP_InitiateRSP
#define RomClosed RSP_RomClosed

#define ConfigSetDefaultBool RSP_ConfigSetDefaultBool
#define ConfigGetParamBool RSP_ConfigGetParamBool
#define ConfigDeleteSection RSP_ConfigDeleteSection
#define ConfigOpenSection RSP_ConfigOpenSection
#define ConfigGetParameter RSP_ConfigGetParameter
#define ConfigSetParameter RSP_ConfigSetParameter
#define ConfigSetDefaultFloat RSP_ConfigSetDefaultFloat
#define ConfigSetDefaultBool RSP_ConfigSetDefaultBool

#define CoreDoCommand RSP_CoreDoCommand

#include "../../mupen64plus-rsp-cxd4/module.c"
