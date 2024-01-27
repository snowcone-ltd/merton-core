#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main/rom.h"
#include "osal/files.h"

#include "../../rom-db.h"

#include "SDL.h"

static SDL_Mutex *OSAL_MUTEX;
static SDL_Condition *OSAL_COND;
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
	return filename;
}

const char *osal_get_user_configpath(void)
{
	return "";
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
		uLongf usize = M64P_DB_USIZE;
		void *ubuf = malloc(usize);

		if (uncompress(ubuf, &usize, M64P_DB, sizeof(M64P_DB)) == Z_OK) {
			fwrite(ubuf, 1, usize, tmp);
			rewind(tmp);
		}

		free(ubuf);

	} else {
		const char *ext = strrchr(filename, '.');

		if (ext && ((!strcmp(ext, "eep") && ROM_SETTINGS.savetype == SAVETYPE_EEPROM_4K) ||
			(!strcmp(ext, "eep") && ROM_SETTINGS.savetype == SAVETYPE_EEPROM_16K) ||
			(!strcmp(ext, "sra") && ROM_SETTINGS.savetype == SAVETYPE_SRAM) ||
			(!strcmp(ext, "fla") && ROM_SETTINGS.savetype == SAVETYPE_FLASH_RAM)))
		{
			fwrite(OSAL_READ_BUF, 1, OSAL_READ_SIZE, tmp);
			rewind(tmp);

		} else {
			// core_log("fopen [%s]: (%s) %s\n", mode, ext, filename);
		}
	}

	return tmp;
}

gzFile osal_gzopen(const char *filename, const char *mode)
{
	struct gzFile_s *f = calloc(1, sizeof(struct gzFile_s));

	free(OSAL_WRITE_BUF);
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

	SDL_LockMutex(OSAL_MUTEX);
	SDL_SignalCondition(OSAL_COND);
	SDL_UnlockMutex(OSAL_MUTEX);

	return 0;
}

int gzwrite(gzFile file, voidpc buf, unsigned len)
{
	if ((size_t) file->pos + len > OSAL_WRITE_SIZE) {
		OSAL_WRITE_SIZE = file->pos + len;
		OSAL_WRITE_BUF = realloc(OSAL_WRITE_BUF, OSAL_WRITE_SIZE);
	}

	memcpy(OSAL_WRITE_BUF + file->pos, buf, len);
	file->pos += len;

	return len;
}


// Shim

void osal_startup(void)
{
	OSAL_MUTEX = SDL_CreateMutex();
	OSAL_COND = SDL_CreateCondition();
}

void osal_shutdown(void)
{
	SDL_DestroyMutex(OSAL_MUTEX);
	OSAL_MUTEX = NULL;

	SDL_DestroyCondition(OSAL_COND);
	OSAL_COND = NULL;

	free(OSAL_READ_BUF);
	OSAL_READ_BUF = NULL;

	free(OSAL_WRITE_BUF);
	OSAL_WRITE_BUF = NULL;
}

void osal_lock(void)
{
	SDL_LockMutex(OSAL_MUTEX);
}

void osal_unlock(void)
{
	SDL_UnlockMutex(OSAL_MUTEX);
}

bool osal_wait(int32_t timeout)
{
	SDL_WaitCondition(OSAL_COND, OSAL_MUTEX);

	return true;
}

const void *osal_get_write_buf(size_t *size)
{
	*size = OSAL_WRITE_SIZE;

	return OSAL_WRITE_BUF;
}

void osal_set_read_data(const void *buf, size_t size)
{
	free(OSAL_READ_BUF);

	OSAL_READ_BUF = malloc(size);
	memcpy(OSAL_READ_BUF, buf, size);

	OSAL_READ_SIZE = size;
}
