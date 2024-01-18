#include "core/n64video.h"

static struct n64video_frame_buffer VDAC_FB;
static uint32_t VDAC_PIXELS[1024][1024];

static void (*CORE_VIDEO)(void *pixels, uint32_t width, uint32_t height, void *opaque);
static void *CORE_VIDEO_OPAQUE;

void vdac_set_video_func(void (*func)(void *, uint32_t, uint32_t, void *), void *opaque)
{
	CORE_VIDEO = func;
	CORE_VIDEO_OPAQUE = opaque;
}

void vdac_init(struct n64video_config* config)
{
}

void vdac_read(struct n64video_frame_buffer* fb, bool alpha)
{
}

void vdac_write(struct n64video_frame_buffer* fb)
{
	memcpy(VDAC_PIXELS, fb->pixels, fb->width * fb->height * 4);

	VDAC_FB = *fb;
}

void vdac_sync(bool valid)
{
	if (!valid)
		memset(VDAC_PIXELS, 0, sizeof(VDAC_PIXELS));

	if (CORE_VIDEO)
		CORE_VIDEO(VDAC_PIXELS, VDAC_FB.width, VDAC_FB.height, CORE_VIDEO_OPAQUE);
}

void vdac_close(void)
{
}
