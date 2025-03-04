#include "FileStream.h"

namespace Mednafen
{


FileStream::FileStream(const std::string& path, const uint32 mode, const int do_lock, const uint32 buffer_size)
	: fd(-1), pos(0),
	  buf(buffer_size ? new uint8[buffer_size] : nullptr), buf_size(buffer_size), buf_write_offs(0), buf_read_offs(0), buf_read_avail(0),
	  need_real_seek(false), locked(false), mapping(NULL), mapping_size(0),
	  OpenedMode(mode), path_humesc(MDFN_strhumesc(path))
{
}

FileStream::~FileStream()
{
}

uint64 FileStream::attributes(void)
{
	return 0;
}

uint8 *FileStream::map(void) noexcept
{
	return NULL;
}

uint64 FileStream::map_size(void) noexcept
{
	return 0;
}

void FileStream::unmap(void) noexcept
{
}

void FileStream::set_buffer_size(uint32 new_size)
{
}

uint64 FileStream::read_ub(void* data, uint64 count)
{
	return 0;
}

uint64 FileStream::write_ub(const void* data, uint64 count)
{
	return 0;
}

void FileStream::write_buffered_data(void)
{
}

void FileStream::write(const void *data, uint64 count)
{
}

uint64 FileStream::read(void *data, uint64 count, bool error_on_eos)
{
	return 0;
}

void FileStream::truncate(uint64 length)
{
}

void FileStream::seek(int64 offset, int whence)
{
}

void FileStream::flush(void)
{
}

uint64 FileStream::tell(void)
{
	return 0;
}

uint64 FileStream::size(void)
{
	return 0;
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
	return 0;
}

}
