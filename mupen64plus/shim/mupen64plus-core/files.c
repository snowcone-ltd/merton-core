#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osal/files.h"

#include "matoya.h"

static char OSAL_DIR[2048];
static MTY_Mutex *OSAL_MUTEX;
static MTY_Cond *OSAL_COND;
static uint8_t *OSAL_WRITE_BUF;
static uint8_t *OSAL_READ_BUF;
static size_t OSAL_WRITE_SIZE;
static size_t OSAL_READ_SIZE;

void core_log(const char *fmt, ...);

int osal_mkdirp(const char *dirpath, int mode)
{
	return 0;
}

const char *osal_get_shared_filepath(const char *filename, const char *firstsearch, const char *secondsearch)
{
	return MTY_JoinPath(OSAL_DIR, filename);
}

const char *osal_get_user_configpath(void)
{
	return MTY_JoinPath(OSAL_DIR, "");
}

const char *osal_get_user_datapath(void)
{
	return osal_get_user_configpath();
}

const char *osal_get_user_cachepath(void)
{
	return osal_get_user_configpath();
}

FILE *osal_file_open(const char *filename, const char *mode)
{
	FILE *tmp = tmpfile();

	if (!strcmp(filename, "memory")) {
		uint8_t magic[4] = {0x1F, 0x8B, 0, 0};
		fwrite(magic, 1, 4, tmp);
		rewind(tmp);

	} else if (strstr(filename, "mupen64plus.ini")) {
		// TODO This could be embedded in the binary
		size_t size = 0;
		void *buf = MTY_ReadFile(filename, &size);

		if (buf) {
			fwrite(buf, 1, size, tmp);
			rewind(tmp);
		}
	} else {
		core_log("fopen: %s\n", filename);
	}

	return tmp;
}

gzFile osal_gzopen(const char *filename, const char *mode)
{
	struct gzFile_s *f = calloc(1, sizeof(struct gzFile_s));

	MTY_Free(OSAL_WRITE_BUF);
	OSAL_WRITE_BUF = NULL;
	OSAL_WRITE_SIZE = 0;

	return f;
}


// Shimmed zlib

int gzread(gzFile file, voidp buf, unsigned len)
{
	int rsize = len;

	if ((size_t) file->pos + len > OSAL_READ_SIZE)
		rsize = OSAL_READ_SIZE - file->pos;

	memcpy(buf, OSAL_READ_BUF + file->pos, rsize);
	file->pos += rsize;

	return rsize;
}

int gzclose(gzFile file)
{
	free(file);

	MTY_MutexLock(OSAL_MUTEX);
	MTY_CondSignal(OSAL_COND);
	MTY_MutexUnlock(OSAL_MUTEX);

	return 0;
}

int gzwrite(gzFile file, voidpc buf, unsigned len)
{
	if ((size_t) file->pos + len > OSAL_WRITE_SIZE) {
		OSAL_WRITE_SIZE = file->pos + len;
		OSAL_WRITE_BUF = MTY_Realloc(OSAL_WRITE_BUF, 1, OSAL_WRITE_SIZE);
	}

	memcpy(OSAL_WRITE_BUF + file->pos, buf, len);
	file->pos += len;

	return len;
}


// Shim

void osal_set_dir(const char *dir)
{
	snprintf(OSAL_DIR, 2048, "%s", dir);
}

void osal_startup(void)
{
	OSAL_MUTEX = MTY_MutexCreate();
	OSAL_COND = MTY_CondCreate();
}

void osal_shutdown(void)
{
	MTY_MutexDestroy(&OSAL_MUTEX);
	MTY_CondDestroy(&OSAL_COND);

	MTY_Free(OSAL_READ_BUF);
	OSAL_READ_BUF = NULL;

	MTY_Free(OSAL_WRITE_BUF);
	OSAL_WRITE_BUF = NULL;
}

void osal_lock(void)
{
	MTY_MutexLock(OSAL_MUTEX);
}

void osal_unlock(void)
{
	MTY_MutexUnlock(OSAL_MUTEX);
}

bool osal_wait(int32_t timeout)
{
	return MTY_CondWait(OSAL_COND, OSAL_MUTEX, timeout);
}

const void *osal_get_write_buf(size_t *size)
{
	*size = OSAL_WRITE_SIZE;

	return OSAL_WRITE_BUF;
}

void osal_set_read_data(const void *buf, size_t size)
{
	MTY_Free(OSAL_READ_BUF);

	OSAL_READ_BUF = MTY_Dup(buf, size);
	OSAL_READ_SIZE = size;
}
