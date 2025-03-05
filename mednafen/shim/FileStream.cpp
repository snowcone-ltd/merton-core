#include "FileStream.h"

#include <sys/stat.h>

void core_log(const char *fmt, ...);

namespace Mednafen
{


FileStream::FileStream(const std::string& path, const uint32 mode, const int do_lock, const uint32 buffer_size)
	: OpenedMode(mode), path_humesc(path)
{
	this->pos = 0;
	this->mapping_size = 0;
	this->mapping = NULL;

	if (mode == MODE_READ) {
		FILE *f = fopen(path.c_str(), "r");

		if (f) {
			this->mapping_size = this->size();
			this->mapping = calloc(this->mapping_size, 1);
			fread(this->mapping, 1, this->mapping_size, f);
			fclose(f);

		} else {
			throw MDFN_Error(errno, "File does not exist");
		}

	} else {
		throw MDFN_Error(-1, "Unsupported file mode");
	}
}

FileStream::~FileStream()
{
	free(this->mapping);
}

uint64 FileStream::attributes(void)
{
	uint64 ret = ATTRIBUTE_SEEKABLE;

	switch ((int64) OpenedMode) {
		case MODE_READ:
			ret |= ATTRIBUTE_READABLE;
			break;
		case MODE_READ_WRITE:
			ret |= ATTRIBUTE_READABLE | ATTRIBUTE_WRITEABLE;
			break;
		case MODE_WRITE_INPLACE:
		case MODE_WRITE_SAFE:
		case MODE_WRITE:
			ret |= ATTRIBUTE_WRITEABLE;
		break;
	}

	return ret;
}

uint8 *FileStream::map(void) noexcept
{
	core_log("*** FileStream map\n");
	return NULL;
}

uint64 FileStream::map_size(void) noexcept
{
	core_log("*** FileStream map_size\n");
	return 0;
}

void FileStream::unmap(void) noexcept
{
	core_log("*** FileStream unmap\n");
}

void FileStream::set_buffer_size(uint32 new_size)
{
	core_log("*** FileStream set_buffer_size\n");
}

uint64 FileStream::read_ub(void* data, uint64 count)
{
	core_log("*** FileStream read_ub\n");
	return 0;
}

uint64 FileStream::write_ub(const void* data, uint64 count)
{
	core_log("*** FileStream write_ub\n");
	return 0;
}

void FileStream::write_buffered_data(void)
{
	core_log("*** FileStream write_buffered_data\n");
}

void FileStream::write(const void *data, uint64 count)
{
	core_log("*** FileStream write\n");
}

uint64 FileStream::read(void *data, uint64 count, bool error_on_eos)
{
	uint64 remaining = this->mapping_size - this->pos;
	uint64 size = count > remaining ? remaining : count;

	memcpy(data, (uint8 *) this->mapping + this->pos, size);
	this->pos += size;

	return size;
}

void FileStream::truncate(uint64 length)
{
	core_log("*** FileStream truncate\n");
}

void FileStream::seek(int64 offset, int whence)
{
	if (offset < 0)
		return;

	if (whence == SEEK_CUR) {
		this->pos += offset;

	} else if (whence == SEEK_SET) {
		this->pos = offset;
	}
}

void FileStream::flush(void)
{
	core_log("*** FileStream flush\n");
}

uint64 FileStream::tell(void)
{
	return this->pos;
}

uint64 FileStream::size(void)
{
	struct stat st;
	if (stat(this->path_humesc.c_str(), &st) != 0)
		return 0;

	return st.st_size;
}

void FileStream::lock(bool nb)
{
	core_log("*** FileStream lock\n");
}

void FileStream::unlock(void)
{
	core_log("*** FileStream unlock\n");
}

void FileStream::close(void)
{
}

int FileStream::get_line(std::string &str)
{
	core_log("*** FileStream get_line\n");
	return 0;
}

}
