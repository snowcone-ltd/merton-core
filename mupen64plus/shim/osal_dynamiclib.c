#include "m64p_types.h"

#include <stdio.h>

#if defined(_WIN32)
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

static char PREFIX[32];

void osal_dynlib_set_prefix(const char *prefix)
{
	snprintf(PREFIX, 32, "%s", prefix);
}

m64p_dynlib_handle osal_dynlib_get_handle(const char *name)
{
	#if defined(_WIN32)
		return (m64p_dynlib_handle) LoadLibrary(name);
	#else
		return (m64p_dynlib_handle) dlopen(name, RTLD_NOW | RTLD_LOCAL);
	#endif
}

void osal_dynlib_close_handle(m64p_dynlib_handle h)
{
	#if defined(_WIN32)
		FreeLibrary(h);
	#else
		dlclose(h);
	#endif
}

void *osal_dynlib_getproc(m64p_dynlib_handle LibHandle, const char *pccProcedureName)
{
	if (!pccProcedureName)
		return NULL;

	char name[1024];
	snprintf(name, 1024, "%s%s", PREFIX, pccProcedureName);

	#if defined(_WIN32)
		return GetProcAddress(LibHandle, name);
	#else
		return dlsym(LibHandle, name);
	#endif
}
