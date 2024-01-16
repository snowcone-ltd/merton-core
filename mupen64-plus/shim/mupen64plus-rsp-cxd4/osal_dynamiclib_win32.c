#include "m64p_types.h"

#include <stdio.h>

static char PREFIX[32] = {0};

void osal_dynlib_set_prefix(const char *prefix)
{
	snprintf(PREFIX, 32, "%s", prefix);
}

m64p_dynlib_handle osal_dynlib_get_handle(const char *name)
{
	return GetModuleHandle(name);
}

void *osal_dynlib_getproc(m64p_dynlib_handle LibHandle, const char *pccProcedureName)
{
	if (!pccProcedureName)
		return NULL;

	char name[1024];
	snprintf(name, 1024, "%s%s", PREFIX, pccProcedureName);

	return GetProcAddress(GetModuleHandle("mupen64plus.dll"), name);
}
