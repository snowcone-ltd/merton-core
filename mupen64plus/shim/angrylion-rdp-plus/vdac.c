#include "core/n64video.h"

static struct n64video_frame_buffer VDAC_FB;
static uint32_t VDAC_PIXELS[1024][1024];

void vdac_init(struct n64video_config* config)
{
}

void vdac_read(struct n64video_frame_buffer* fb, bool alpha)
{
	memcpy(fb->pixels, VDAC_PIXELS, VDAC_FB.width * VDAC_FB.height * 4);

	fb->width = VDAC_FB.width;
	fb->height = VDAC_FB.height;
	fb->height_out = VDAC_FB.height_out;
	fb->pitch = VDAC_FB.pitch;
	fb->valid = VDAC_FB.valid;
}

void vdac_write(struct n64video_frame_buffer* fb)
{
	memcpy(VDAC_PIXELS, fb->pixels, fb->width * fb->height * 4);

	VDAC_FB = *fb;
	VDAC_FB.pixels = NULL;
}

void vdac_sync(bool invaid)
{
}

void vdac_close(void)
{
}
