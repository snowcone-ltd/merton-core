#include "GZFileStream.h"

namespace Mednafen
{

GZFileStream::GZFileStream(const std::string& path, const MODE mode, const int level) : OpenedMode(mode), path_humesc(MDFN_strhumesc(path))
{
}

GZFileStream::~GZFileStream()
{
}

void GZFileStream::close(void)
{
}

uint64 GZFileStream::read(void *data, uint64 count, bool error_on_eof)
{
	return 0;
}

int GZFileStream::get_line(std::string &str)
{
	return 0;
}

void GZFileStream::flush(void)
{
}

void GZFileStream::write(const void *data, uint64 count)
{
}

void GZFileStream::truncate(uint64 length)
{
}

void GZFileStream::seek(int64 offset, int whence)
{
}

uint64 GZFileStream::size(void)
{
	return 0;
}

uint64 GZFileStream::tell(void)
{
	return 0;
}


uint64 GZFileStream::attributes(void)
{
	return 0;
}

}
