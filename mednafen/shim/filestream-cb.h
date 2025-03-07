#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef void (*FILE_STREAM_WRITE_CB)(const char *path, const void *buf, uint64_t size, void *opaque);
typedef bool (*FILE_STREAM_READ_CB)(const char *path, void **buf, uint64_t *size, void *opaque);

void file_stream_set_callbacks(FILE_STREAM_WRITE_CB write_cb, FILE_STREAM_READ_CB read_cb, void *opaque);
