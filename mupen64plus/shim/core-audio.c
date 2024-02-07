#include "m64p_plugin.h"

#include "../../core.h"

static uint32_t SAMPLE_RATE;
static AUDIO_INFO AI;

static CoreAudioFunc CORE_AUDIO;
static void *CORE_AUDIO_OPAQUE;

static int16_t REV_BUF[8 * 1024];

void audio_set_func(CoreAudioFunc func, void *opaque)
{
	CORE_AUDIO = func;
	CORE_AUDIO_OPAQUE = opaque;
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
	int16_t *buf = (int16_t *) (AI.RDRAM + (*AI.AI_DRAM_ADDR_REG & 0xFFFFFF));
	size_t frames = *AI.AI_LEN_REG / 4;

	for (size_t x = 0; x < frames * 2; x += 2) {
		REV_BUF[x] = buf[x + 1];
		REV_BUF[x + 1] = buf[x];
	}

	CORE_AUDIO(REV_BUF, frames, SAMPLE_RATE, CORE_AUDIO_OPAQUE);
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
