#include "../core.h"

#include <stdarg.h>

#include "shared.h"
#include "md_ntsc.h"
#include "sms_ntsc.h"

#include "shim/osd.h"

#include "settings.h"

#define CORE_BITMAP_W 720
#define CORE_BITMAP_H 576
#define CORE_SAMPLE_RATE 48000

struct Core {
	int16 audio_buffer[2048];
	uint16_t bitmap_data[CORE_BITMAP_H][CORE_BITMAP_W];
};

static CoreLogFunc CORE_LOG;
static CoreAudioFunc CORE_AUDIO;
static CoreVideoFunc CORE_VIDEO;
static void *CORE_LOG_OPAQUE;
static void *CORE_AUDIO_OPAQUE;
static void *CORE_VIDEO_OPAQUE;


// From "osd.h"

void core_log(const char *fmt, ...);

struct config config;

char GG_ROM[1024];
char AR_ROM[1024];
char SK_ROM[1024];
char SK_UPMEM[1024];
char GG_BIOS[1024];
char CD_BIOS_EU[1024];
char CD_BIOS_US[1024];
char CD_BIOS_JP[1024];
char MS_BIOS_US[1024];
char MS_BIOS_EU[1024];
char MS_BIOS_JP[1024];

static md_ntsc_t _md_ntsc = {0};
md_ntsc_t *md_ntsc = &_md_ntsc;

static sms_ntsc_t _sms_ntsc = {0};
sms_ntsc_t *sms_ntsc = &_sms_ntsc;

void osd_input_update(void)
{
	// TODO Make it atomic
}

int load_archive(char *filename, unsigned char *buffer, int maxsize, char *extension)
{
	FILE *f = fopen(filename, "rb");
	if (!f)
		return 0;

	void *file = malloc(maxsize);

	size_t size = fread(file, 1, maxsize, f);
	memcpy(buffer, file, size);

	free(file);
	fclose(f);

	return (int) size;
}

long crc32(unsigned long crc, const unsigned char *buf, unsigned int len)
{
	crc = ~crc;

	for (uint32_t x = 0; x < len; x++) {
		crc = crc ^ buf[x];

		for (uint8_t y = 0; y < 8; y++)
			crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
	}

	return ~crc;
}


// Core interface

static const uint8_t CORE_BRM_FORMAT[0x40] = {
	0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x40,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x53, 0x45, 0x47, 0x41, 0x5F, 0x43, 0x44, 0x5F, 0x52, 0x4F, 0x4D, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x52, 0x41, 0x4D, 0x5F, 0x43, 0x41, 0x52, 0x54, 0x52, 0x49, 0x44, 0x47, 0x45, 0x5F, 0x5F, 0x5F,
};

void error(char *fmt, ...)
{
	// TODO This shows very verbose low level information
}

void core_log(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	char msg[1024];
	vsnprintf(msg, 1024, fmt, arg);

	CORE_LOG(msg, CORE_LOG_OPAQUE);

	va_end(arg);
}

static void core_set_settings(void)
{
	memset(&config, 0, sizeof(config));

	// Sound
	config.psg_preamp = 150;
	config.fm_preamp = 100;
	config.cdda_volume = 100;
	config.pcm_volume = 100;
	config.hq_fm = 1;
	config.hq_psg = 1;
	config.filter = CMP_ENUM("lpf", "3-Pass") ? 2 :
		CMP_ENUM("lpf", "Simple") ? 1 : 0;
	config.mono = !CMP_BOOL("stereo");
	config.low_freq = 200;
	config.high_freq = 8000;
	config.lg = 100;
	config.mg = 100;
	config.hg = 100;
	config.lp_range = 0x9999;
	config.ym2612 = YM2612_ENHANCED;
	// config.ym2612 = YM2612_INTEGRATED;
	// config.ym2612 = YM2612_DISCRETE;
	config.ym3438 = ym3438_mode_readmode;
	// config.ym3438 = ym3438_mode_ym2612;

	YM2612Config(config.ym2612);
	OPN2_SetChipType(config.ym3438);

	// System
	config.addr_error = 1;
	config.cd_latency = 1;
	config.no_sprite_limit = CMP_BOOL("no-sprite-limit");

	// Display
	config.enhanced_vscroll_limit = 8;
	config.ntsc = CMP_ENUM("ntsc", "Monochrome") ? 1 : CMP_ENUM("ntsc", "Composite") ? 2 :
		CMP_ENUM("ntsc", "S-Video") ? 3 : CMP_ENUM("ntsc", "RGB") ? 4 : 0;

	// Input
	for (uint8_t x = 0; x < MAX_INPUTS; x++)
		config.input[x].padtype = DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B;

	// NTSC filter
	switch (config.ntsc) {
		case 1:
			md_ntsc_init(md_ntsc, &md_ntsc_monochrome);
			sms_ntsc_init(sms_ntsc, &sms_ntsc_monochrome);
			break;
		case 2:
			md_ntsc_init(md_ntsc, &md_ntsc_composite);
			sms_ntsc_init(sms_ntsc, &sms_ntsc_composite);
			break;
		case 3:
			md_ntsc_init(md_ntsc, &md_ntsc_svideo);
			sms_ntsc_init(sms_ntsc, &sms_ntsc_svideo);
			break;
		case 4:
			md_ntsc_init(md_ntsc, &md_ntsc_rgb);
			sms_ntsc_init(sms_ntsc, &sms_ntsc_rgb);
			break;
	}
}

void CoreUnloadGame(Core **core)
{
	if (!core || !*core)
		return;

	audio_shutdown();

	free(*core);
	*core = NULL;
}

void CoreSetLogFunc(CoreLogFunc func, void *opaque)
{
	CORE_LOG = func;
	CORE_LOG_OPAQUE = opaque;
}

void CoreSetAudioFunc(CoreAudioFunc func, void *opaque)
{
	CORE_AUDIO = func;
	CORE_AUDIO_OPAQUE = opaque;
}

void CoreSetVideoFunc(CoreVideoFunc func, void *opaque)
{
	CORE_VIDEO = func;
	CORE_VIDEO_OPAQUE = opaque;
}

static void core_mcd_format_bram(uint8_t *bram, uint32_t size)
{
	if (memcmp(bram + size - 0x20, CORE_BRM_FORMAT + 0x20, 0x20)) {
		uint8_t *ptr = bram + size - 0x40;
		memcpy(ptr, CORE_BRM_FORMAT, 0x40);
		memset(bram, 0x00, size - 0x40);

		uint16_t page_size = size / 64 - 3;

		for (uint8_t x = 0x10; x < 0x18; x += 2) {
			ptr[x] = ((uint8_t *) &page_size)[1];
			ptr[x + 1] = ((uint8_t *) &page_size)[0];
		}
	}
}

static void core_load_save_data(const void *data, size_t size)
{
	size_t csize = 0x10000;
	void *ptr = sram.sram;

	if (system_hw == SYSTEM_MCD) {
		ptr = scd.bram;
		csize = 0x2000;

		core_mcd_format_bram(scd.bram, 0x2000);

		// XXX External cart is not used, how to disable it?
		if (scd.cartridge.id)
			core_mcd_format_bram(scd.cartridge.area, 0x80000);
	}

	if (data && size <= csize)
		memcpy(ptr, data, size);
}

Core *CoreLoadGame(CoreSystem system, const char *systemDir, const char *path,
	const void *saveData, size_t saveDataSize)
{
	Core *ctx = calloc(1, sizeof(Core));

	#if defined(_WIN32)
		const char *delim = "\\";
	#else
		const char *delim = "/";
	#endif

	bool has_delim = systemDir[strlen(systemDir) - 1] == delim[0];

	#define FILL_BIOS(var, name) \
		snprintf(var, sizeof(var), "%s%s%s", systemDir, has_delim ? "" : delim, name)

	FILL_BIOS(GG_ROM, "ggenie.bin");
	FILL_BIOS(GG_BIOS, "bios.gg");
	FILL_BIOS(AR_ROM, "areplay.bin");
	FILL_BIOS(SK_ROM, "sk.bin");
	FILL_BIOS(SK_UPMEM, "sk2chip.bin");
	FILL_BIOS(CD_BIOS_US, "bios_CD_U.bin");
	FILL_BIOS(CD_BIOS_EU, "bios_CD_E.bin");
	FILL_BIOS(CD_BIOS_JP, "bios_CD_J.bin");
	FILL_BIOS(MS_BIOS_US, "bios_U.sms");
	FILL_BIOS(MS_BIOS_EU, "bios_E.sms");
	FILL_BIOS(MS_BIOS_JP, "bios_J.sms");

	bitmap.width = CORE_BITMAP_W;
	bitmap.height = CORE_BITMAP_H;
	bitmap.pitch = CORE_BITMAP_W * sizeof(uint16_t);
	bitmap.data = (unsigned char *) ctx->bitmap_data;

	input.system[0] = SYSTEM_GAMEPAD;
	input.system[1] = SYSTEM_GAMEPAD;

	core_set_settings();

	if (load_rom((char *) path)) {
		audio_init(CORE_SAMPLE_RATE, 0);

		system_init();
		core_load_save_data(saveData, saveDataSize);

		system_reset();

	} else {
		CoreUnloadGame(&ctx);
	}

	return ctx;
}

double CoreGetFrameRate(Core *ctx)
{
	if (!ctx)
		return 60.0;

	return (double) system_clock / (double) lines_per_frame / (double) MCYCLES_PER_LINE;
}

float CoreGetAspectRatio(Core *ctx)
{
	if (!ctx)
		return 1.0f;

	bool is_h40 = bitmap.viewport.w == 320;
	double dotrate = system_clock / (is_h40 ? 8.0 : 10.0);
	double videosamplerate = 135000000.0 / 11.0;

	return (videosamplerate / dotrate) * ((double) bitmap.viewport.w / ((double) bitmap.viewport.h * 2.0));
}

void CoreRun(Core *ctx)
{
	if (!ctx)
		return;

	if (system_hw == SYSTEM_MCD) {
		system_frame_scd(0);

	} else {
		system_frame_gen(0);
	}

	size_t o = bitmap.viewport.x + bitmap.viewport.y * bitmap.pitch;
	uint32_t w = bitmap.viewport.w;

	// XXX Oddly MD need to use the SMS NTSC filter from time to time
	if (config.ntsc)
		w = reg[12] & 1 ? MD_NTSC_OUT_WIDTH(w) : SMS_NTSC_OUT_WIDTH(w);

	CORE_VIDEO(ctx->bitmap_data + o, CORE_COLOR_FORMAT_B5G6R5, w, bitmap.viewport.h,
		bitmap.pitch, CORE_VIDEO_OPAQUE);

	int frames = audio_update(ctx->audio_buffer);
	CORE_AUDIO(ctx->audio_buffer, frames, CORE_SAMPLE_RATE, CORE_AUDIO_OPAQUE);
}

void *CoreGetSaveData(Core *ctx, size_t *size)
{
	if (!ctx)
		return NULL;

	void *ptr = NULL;

	if (system_hw == SYSTEM_MCD) {
		*size = 0x2000;
		ptr = scd.bram;

	} else if (sram.on) {
		*size = 0x10000;
		ptr = sram.sram;

	} else {
		return NULL;
	}

	void *sd = malloc(*size);
	memcpy(sd, ptr, *size);

	return sd;
}

void CoreReset(Core *ctx)
{
	if (!ctx)
		return;

	gen_reset(0);
}

void CoreSetButton(Core *ctx, uint8_t player, CoreButton button, bool pressed)
{
	if (!ctx || player >= MAX_INPUTS)
		return;

	uint16 b = 0;

	switch (button) {
		case CORE_BUTTON_B: b = INPUT_C; break;
		case CORE_BUTTON_A: b = INPUT_B; break;
		case CORE_BUTTON_X: b = INPUT_A; break;
		case CORE_BUTTON_START: b = INPUT_START; break;
		case CORE_BUTTON_DPAD_U: b = INPUT_UP; break;
		case CORE_BUTTON_DPAD_D: b = INPUT_DOWN; break;
		case CORE_BUTTON_DPAD_L: b = INPUT_LEFT; break;
		case CORE_BUTTON_DPAD_R: b = INPUT_RIGHT; break;
		// case CORE_BUTTON_: b = INPUT_X; break;
		// case CORE_BUTTON_: b = INPUT_Y; break;
		// case CORE_BUTTON_: b = INPUT_Z; break;
		// case CORE_BUTTON_: b = INPUT_MODE; break;
		default:
			break;
	}

	if (pressed) {
		input.pad[player] |= b;

	} else {
		input.pad[player] &= ~b;
	}
}

void CoreSetAxis(Core *ctx, uint8_t player, CoreAxis axis, int16_t value)
{
}

void *CoreGetState(Core *ctx, size_t *size)
{
	if (!ctx)
		return NULL;

	*size = STATE_SIZE;
	void *state = malloc(*size);

	state_save(state);

	return state;
}

bool CoreSetState(Core *ctx, const void *state, size_t size)
{
	if (!ctx || size != STATE_SIZE)
		return false;

	return state_load((unsigned char *) state);
}

bool CoreInsertDisc(Core *ctx, const char *path)
{
	if (system_hw != SYSTEM_MCD)
		return false;

	cdd.status = CD_OPEN;
	scd.regs[0x36 >> 1].byte.h = 0x01;

	char header[0x210];
	cdd_load((char *) path, header);

	cdd.status = cdd.loaded ? CD_TOC : NO_DISC;

	return cdd.loaded;
}

CoreSetting *CoreGetSettings(uint32_t *len)
{
	*len = sizeof(CORE_SETTINGS) / sizeof(CoreSetting);

	return CORE_SETTINGS;
}

void CoreUpdateSettings(Core *ctx)
{
	if (!ctx)
		return;

	core_set_settings();
}
