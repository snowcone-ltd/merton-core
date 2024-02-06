#define GET_VAL(name) \
	core_setting(name)->value

#define CMP_ENUM(name, val) \
	!strcmp(GET_VAL(name), val)

#define CMP_BOOL(name) \
	!strcmp(GET_VAL(name), "true")

static CoreSetting *core_setting(const char *name)
{
	uint32_t len = 0;
	CoreSetting *settings = CoreGetSettings(&len);

	for (uint32_t x = 0; x < len; x++) {
		CoreSetting *s = &settings[x];

		if (!strcmp(s->key, name))
			return s;
	}

	return NULL;
}

static CoreSetting CORE_SETTINGS[] = {{
	CORE_SYSTEM_GENESIS,
	CORE_SETTING_ENUM,
	CORE_SETTING_GROUP_AUDIO,
	"Low-Pass Filter",
	"lpf",
	{"Off", "Simple", "3-Pass"},
	"Off",
	3,
}, {
	CORE_SYSTEM_GENESIS,
	CORE_SETTING_ENUM,
	CORE_SETTING_GROUP_VIDEO,
	"NTSC Filter",
	"ntsc",
	{"Off", "Monochrome", "Composite", "S-Video", "RGB"},
	"Off",
	5,
}, {
	CORE_SYSTEM_GENESIS,
	CORE_SETTING_BOOL,
	CORE_SETTING_GROUP_AUDIO,
	"Stereo",
	"stereo",
	{""},
	"true",
	0,
}, {
	CORE_SYSTEM_GENESIS,
	CORE_SETTING_BOOL,
	CORE_SETTING_GROUP_VIDEO,
	"Disable Sprite Limit",
	"no-sprite-limit",
	{""},
	"false",
	0,
}};
