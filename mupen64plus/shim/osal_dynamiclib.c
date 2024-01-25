#include "m64p_types.h"

#include <stdio.h>

#include "matoya.h"

static char PREFIX[32];

void osal_dynlib_set_prefix(const char *prefix)
{
	snprintf(PREFIX, 32, "%s", prefix);
}

m64p_dynlib_handle osal_dynlib_get_handle(const char *name)
{
	return (m64p_dynlib_handle) MTY_SOLoad(name);
}

void osal_dynlib_close_handle(m64p_dynlib_handle h)
{
	MTY_SO *so = (MTY_SO *) h;

	MTY_SOUnload(&so);
}

void *osal_dynlib_getproc(m64p_dynlib_handle LibHandle, const char *pccProcedureName)
{
	if (!pccProcedureName)
		return NULL;

	char name[1024];
	snprintf(name, 1024, "%s%s", PREFIX, pccProcedureName);

	return MTY_SOGetSymbol((MTY_SO *) LibHandle, name);
}
