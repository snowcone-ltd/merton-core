#define M64P_CORE_PROTOTYPES
#include "m64p_vidext.h"
#include "vidext.h"

m64p_error OverrideVideoFunctions(m64p_video_extension_functions *VideoFunctionStruct)
{
	return M64ERR_SUCCESS;
}

int VidExt_InFullscreenMode(void)
{
	return 0;
}

int VidExt_VideoRunning(void)
{
	return 1;
}

m64p_error VidExt_GL_SwapBuffers(void)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_Init(void)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_Quit(void)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_ListFullscreenModes(m64p_2d_size *a, int *b)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_SetVideoMode(int a, int b, int c, m64p_video_mode d, m64p_video_flags e)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_SetCaption(const char *a)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_ToggleFullScreen(void)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_ResizeWindow(int a, int b)
{
	return M64ERR_SUCCESS;
}

m64p_function CALL VidExt_GL_GetProcAddress(const char *a)
{
	return NULL;
}

m64p_error CALL VidExt_GL_SetAttribute(m64p_GLattr a, int b)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_GL_GetAttribute(m64p_GLattr a, int *b)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_InitWithRenderMode(m64p_render_mode a)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_ListFullscreenRates(m64p_2d_size a, int *b, int *c)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_SetVideoModeWithRate(int a, int b, int c, int d, m64p_video_mode e, m64p_video_flags f)
{
	return M64ERR_SUCCESS;
}

uint32_t CALL VidExt_GL_GetDefaultFramebuffer(void)
{
	return 0;
}

m64p_error CALL VidExt_VK_GetSurface(void** a, void* b)
{
	return M64ERR_SUCCESS;
}

m64p_error CALL VidExt_VK_GetInstanceExtensions(const char**a[], uint32_t* b)
{
	return M64ERR_SUCCESS;
}
