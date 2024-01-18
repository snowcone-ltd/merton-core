#include "m64p_plugin.h"

#include "../core.h"

static uint32_t SAMPLE_RATE;
static AUDIO_INFO AI;

static CoreAudioFunc CORE_AUDIO;
static void *CORE_AUDIO_OPAQUE;

void audio_set_callback(CoreAudioFunc func, void *opaque)
{
	CORE_AUDIO = func;
	CORE_AUDIO_OPAQUE = opaque;
}

EXPORT m64p_error AUDIO_PluginStartup(m64p_dynlib_handle CoreLibHandle, void *Context,
	void (*DebugCallback)(void *, int, const char *))
{
	return M64ERR_SUCCESS;
}

EXPORT m64p_error AUDIO_PluginShutdown(void)
{
	return M64ERR_SUCCESS;
}

EXPORT m64p_error AUDIO_PluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion,
	const char **PluginNamePtr, int *Capabilities)
{
	if (PluginType)
		*PluginType = M64PLUGIN_AUDIO;

	if (PluginVersion)
		*PluginVersion = 0x010000;

	if (APIVersion)
		*APIVersion = 0x020000;

	if (PluginNamePtr)
		*PluginNamePtr = "Core Audio";

	if (Capabilities)
		*Capabilities = 0;

	return M64ERR_SUCCESS;
}

EXPORT void AUDIO_AiDacrateChanged(int SystemType)
{
	uint32_t vi_clock = 48681812; // NTSC
	// uint32_t vi_clock = 49656530; // PAL
	// uint32_t vi_clock = 48628316; // M-PAL

	SAMPLE_RATE = vi_clock / (*AI.AI_DACRATE_REG + 1);
}

EXPORT void AUDIO_AiLenChanged(void)
{
	void *buf = AI.RDRAM + (*AI.AI_DRAM_ADDR_REG & 0xFFFFFF);
	size_t frames = *AI.AI_LEN_REG / 4;

	if (CORE_AUDIO)
		CORE_AUDIO(buf, frames, SAMPLE_RATE, CORE_AUDIO_OPAQUE);
}

EXPORT int AUDIO_InitiateAudio(AUDIO_INFO Audio_Info)
{
	AI = Audio_Info;

	return 1;
}

EXPORT int AUDIO_RomOpen(void)
{
	return 1;
}

EXPORT void AUDIO_RomClosed(void)
{
}

EXPORT void AUDIO_ProcessAList(void)
{
}

EXPORT void AUDIO_SetSpeedFactor(int percentage)
{
}

EXPORT void AUDIO_VolumeMute(void)
{
}

EXPORT void AUDIO_VolumeUp(void)
{
}

EXPORT void AUDIO_VolumeDown(void)
{
}

EXPORT int AUDIO_VolumeGetLevel(void)
{
	return 100;
}

EXPORT void AUDIO_VolumeSetLevel(int level)
{
}

EXPORT const char *AUDIO_VolumeGetString(void)
{
	return "100%";
}