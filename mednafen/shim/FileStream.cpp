#include "FileStream.h"

#include <sys/stat.h>

#include "filestream-cb.h"

using namespace Mednafen;

void core_log(const char *fmt, ...);

static FILE_STREAM_WRITE_CB FILE_STREAM_WRITE;
static FILE_STREAM_READ_CB FILE_STREAM_READ;
static void *FILE_STREAM_OPAQUE;

void file_stream_set_callbacks(FILE_STREAM_WRITE_CB write_cb, FILE_STREAM_READ_CB read_cb, void *opaque)
{
	FILE_STREAM_WRITE = write_cb;
	FILE_STREAM_READ = read_cb;
	FILE_STREAM_OPAQUE = opaque;
}

FileStream::FileStream(const std::string& path, const uint32 mode, const int do_lock, const uint32 buffer_size)
	: pos(0), mapping(NULL), mapping_size(0), OpenedMode(mode), path_humesc(MDFN_strhumesc(path))
{
	if (mode == MODE_READ) {
		if (FILE_STREAM_READ)
			if (FILE_STREAM_READ(path.c_str(), &this->mapping, &this->mapping_size, FILE_STREAM_OPAQUE))
				return;

		const char *cpath = path.c_str();
		struct stat st;

		if (stat(cpath, &st) == 0) {
			FILE *f = fopen(cpath, "rb");

			if (f) {
				this->mapping = malloc(st.st_size);

				if (fread(this->mapping, 1, st.st_size, f) == st.st_size)
					this->mapping_size = st.st_size;

				fclose(f);
			}
		}

		if (this->mapping_size == 0)
			throw MDFN_Error(errno, "File does not exist");

	} else if (mode == MODE_READ_WRITE) {
		throw MDFN_Error(0, "Unsupported file mode: MODE_READ_WRITE: %s", path.c_str());
	}
}

FileStream::~FileStream()
{
	free(this->mapping);
}

uint64 FileStream::attributes(void)
{
	return ATTRIBUTE_SEEKABLE |
		(this->OpenedMode == MODE_READ ? ATTRIBUTE_READABLE : ATTRIBUTE_WRITEABLE);
}

void FileStream::write(const void *data, uint64 count)
{
	this->pos += count;

	if (FILE_STREAM_WRITE)
		FILE_STREAM_WRITE(this->path_humesc.c_str(), data, count, FILE_STREAM_OPAQUE);
}

uint64 FileStream::read(void *data, uint64 count, bool error_on_eos)
{
	uint64 rem = this->mapping_size - this->pos;
	uint64 n = rem < count ? rem : count;

	memcpy(data, (uint8 *) this->mapping + this->pos, n);
	this->pos += n;

	if (n != count && error_on_eos)
		throw MDFN_Error(0, "Unexpected EOF");

	return n;
}

void FileStream::seek(int64 offset, int whence)
{
	if (whence == SEEK_CUR) {
		this->pos += offset;

	} else if (whence == SEEK_SET) {
		this->pos = offset;
	}
}

uint64 FileStream::tell(void)
{
	return this->pos;
}

uint64 FileStream::size(void)
{
	return this->mapping_size;
}

void FileStream::close(void)
{
}


// Unimplemented

uint8 *FileStream::map(void) noexcept
{
	core_log("*** FileStream::map\n");

	return NULL;
}

uint64 FileStream::map_size(void) noexcept
{
	core_log("*** FileStream::map_size\n");

	return 0;
}

void FileStream::unmap(void) noexcept
{
	core_log("*** FileStream::unmap\n");
}

void FileStream::write_buffered_data(void)
{
	core_log("*** FileStream::write_buffered_data\n");
}

void FileStream::truncate(uint64 length)
{
	core_log("*** FileStream::truncate\n");
}

void FileStream::flush(void)
{
	core_log("*** FileStream::flush\n");
}

int FileStream::get_line(std::string &str)
{
	core_log("*** FileStream::get_line\n");

	return 0;
}
