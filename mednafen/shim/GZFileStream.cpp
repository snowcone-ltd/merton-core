#include "GZFileStream.h"

void core_log(const char *fmt, ...);

namespace Mednafen
{

GZFileStream::GZFileStream(const std::string& path, const MODE mode, const int level) : OpenedMode(mode), path_humesc(MDFN_strhumesc(path))
{
	core_log("*** GZFileStream\n");
}

GZFileStream::~GZFileStream()
{
	core_log("*** GZFileStream\n");
}

void GZFileStream::close(void)
{
	core_log("*** GZFileStream\n");
}

uint64 GZFileStream::read(void *data, uint64 count, bool error_on_eof)
{
	core_log("*** GZFileStream\n");
	return 0;
}

int GZFileStream::get_line(std::string &str)
{
	core_log("*** GZFileStream\n");
	return 0;
}

void GZFileStream::flush(void)
{
	core_log("*** GZFileStream\n");
}

void GZFileStream::write(const void *data, uint64 count)
{
	core_log("*** GZFileStream\n");
}

void GZFileStream::truncate(uint64 length)
{
	core_log("*** GZFileStream\n");
}

void GZFileStream::seek(int64 offset, int whence)
{
	core_log("*** GZFileStream\n");
}

uint64 GZFileStream::size(void)
{
	core_log("*** GZFileStream\n");
	return 0;
}

uint64 GZFileStream::tell(void)
{
	core_log("*** GZFileStream\n");
	return 0;
}


uint64 GZFileStream::attributes(void)
{
	core_log("*** GZFileStream\n");
	return 0;
}

}
