#pragma once

#define MAX_INPUTS 8

#define HAVE_NO_SPRITE_LIMIT
#define MAX_SPRITES_PER_LINE 80
#define TMS_MAX_SPRITES_PER_LINE (config.no_sprite_limit ? MAX_SPRITES_PER_LINE : 4)
#define MODE4_MAX_SPRITES_PER_LINE (config.no_sprite_limit ? MAX_SPRITES_PER_LINE : 8)
#define MODE5_MAX_SPRITES_PER_LINE (config.no_sprite_limit ? MAX_SPRITES_PER_LINE : (bitmap.viewport.w >> 4))
#define MODE5_MAX_SPRITE_PIXELS (config.no_sprite_limit ? MAX_SPRITES_PER_LINE * 32 : max_sprite_pixels)

void osd_input_update(void);
int load_archive(char *filename, unsigned char *buffer, int maxsize, char *extension);
long crc32(unsigned long crc, const unsigned char *buf, unsigned int len);
void error(char *fmt, ...);

struct config {
	uint8 hq_fm;
	uint8 filter;
	uint8 hq_psg;
	uint8 ym2612;
	uint8 ym2413;
	uint8 ym3438;
	uint8 cd_latency;
	int16 psg_preamp;
	int16 fm_preamp;
	int16 cdda_volume;
	int16 pcm_volume;
	uint32 lp_range;
	int16 low_freq;
	int16 high_freq;
	int16 lg;
	int16 mg;
	int16 hg;
	uint8 mono;
	uint8 system;
	uint8 region_detect;
	uint8 vdp_mode;
	uint8 master_clock;
	uint8 force_dtack;
	uint8 addr_error;
	uint8 bios;
	uint8 lock_on;
	uint8 add_on;
	uint8 overscan;
	uint8 gg_extra;
	uint8 ntsc;
	uint8 lcd;
	uint8 render;
	uint8 enhanced_vscroll;
	uint8 enhanced_vscroll_limit;
	uint8 no_sprite_limit;
	struct {
		uint8 padtype;
	} input[MAX_INPUTS];
};

extern struct config config;

extern char GG_ROM[1024];
extern char GG_BIOS[1024];
extern char AR_ROM[1024];
extern char SK_ROM[1024];
extern char SK_UPMEM[1024];
extern char CD_BIOS_EU[1024];
extern char CD_BIOS_US[1024];
extern char CD_BIOS_JP[1024];
extern char MS_BIOS_US[1024];
extern char MS_BIOS_EU[1024];
extern char MS_BIOS_JP[1024];
