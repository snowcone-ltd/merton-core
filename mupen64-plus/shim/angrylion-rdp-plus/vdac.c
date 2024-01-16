#include "core/n64video.h"

struct n64video_frame_buffer FB;

void vdac_init(struct n64video_config* config)
{
}

void vdac_read(struct n64video_frame_buffer* fb, bool alpha)
{
	fb->width = FB.width;
	fb->height = FB.height;
	fb->height_out = FB.height_out;
	fb->pitch = FB.pitch;
	fb->valid = FB.valid;
}

void vdac_write(struct n64video_frame_buffer* fb)
{
	FB = *fb;
}

void vdac_sync(bool invaid)
{
}

void vdac_close(void)
{
}
