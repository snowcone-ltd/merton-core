#include "core.h"

#define CORE_SETTING_PREFIX "mesen2-"

static const CoreSetting CORE_SETTINGS[] = {{
	.system = CORE_SYSTEM_NES,
	.type = CORE_SETTING_BOOL,
	.group = CORE_SETTING_GROUP_VIDEO,
	.desc = "Filter Garbage on Reset",
	.key = CORE_SETTING_PREFIX "wait-for-pr",
}, {
	.system = CORE_SYSTEM_NES,
	.type = CORE_SETTING_BOOL,
	.group = CORE_SETTING_GROUP_AUDIO,
	.desc = "Triangle High Freq.",
	.key = CORE_SETTING_PREFIX "triangle-pop",
}, {
	.system = CORE_SYSTEM_NES,
	.type = CORE_SETTING_BOOL,
	.group = CORE_SETTING_GROUP_AUDIO,
	.desc = "Stereo",
	.key = CORE_SETTING_PREFIX "stereo",
}};

const CoreSetting *CoreGetAllSettings(Core *ctx, uint32_t *len)
{
	*len = sizeof(CORE_SETTINGS) / sizeof(CoreSetting);

	return CORE_SETTINGS;
}
