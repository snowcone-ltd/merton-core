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
	return 0;
}
