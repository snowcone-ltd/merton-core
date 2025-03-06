#include "FileStream.h"

void core_log(const char *fmt, ...);

namespace Mednafen
{

FileStream::FileStream(const std::string& path, const uint32 mode, const int do_lock, const uint32 buffer_size)
	: pos(0), mapping(NULL), mapping_size(0), OpenedMode(mode), path_humesc(MDFN_strhumesc(path))
{
	if (mode == MODE_READ) {
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
	}
}

FileStream::~FileStream()
{
	free(this->mapping);
}

uint64 FileStream::attributes(void)
{
	uint64 ret = ATTRIBUTE_SEEKABLE;

	switch (this->OpenedMode) {
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
	return (uint8 *) this->mapping;
}

uint64 FileStream::map_size(void) noexcept
{
	return this->mapping_size;
}

void FileStream::unmap(void) noexcept
{
}

void FileStream::set_buffer_size(uint32 new_size)
{
}

uint64 FileStream::read_ub(void* data, uint64 count)
{
	uint64 rem = this->mapping_size - this->pos;
	uint64 n = rem < count ? rem : count;

	memcpy(data, (uint8 *) this->mapping + this->pos, n);
	this->pos += n;

	return n;
}

uint64 FileStream::write_ub(const void* data, uint64 count)
{
	this->pos += count;

	core_log("*** FileStream::write_ub: count=%u, file=%s\n", count, this->path_humesc.c_str());

	return count;
}

void FileStream::write_buffered_data(void)
{
}

void FileStream::write(const void *data, uint64 count)
{
	this->write_ub(data, count);
}

uint64 FileStream::read(void *data, uint64 count, bool error_on_eos)
{
	uint64 n = this->read_ub(data, count);

	if (n != count && error_on_eos)
		throw MDFN_Error(0, "Unexpected EOF");

	return n;
}

void FileStream::truncate(uint64 length)
{
	if (this->OpenedMode == MODE_READ)
		throw MDFN_Error(0, "Error truncating file in MODE_READ");

	if (length < this->pos)
		this->pos = length;
}

void FileStream::seek(int64 offset, int whence)
{
	if (whence == SEEK_CUR) {
		this->pos += offset;

	} else if (whence == SEEK_SET) {
		this->pos = offset;
	}
}

void FileStream::flush(void)
{
}

uint64 FileStream::tell(void)
{
	return this->pos;
}

uint64 FileStream::size(void)
{
	return this->mapping_size;
}

void FileStream::lock(bool nb)
{
}

void FileStream::unlock(void)
{
}

void FileStream::close(void)
{
}

int FileStream::get_line(std::string &str)
{
	int c;

	str.clear();

	while((c = get_char()) >= 0) {
		if(c == '\r' || c == '\n' || c == 0)
			return(c);

		str.push_back(c);
	}

	return(str.length() ? 256 : -1);
}

}
