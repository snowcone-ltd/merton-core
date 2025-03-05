#pragma once

#if defined(_WIN32)
	#include <io.h>

	#define S_IRUSR _S_IREAD
	#define S_IWUSR _S_IWRITE

#else
	#include <unistd.h>
#endif
