#include "mednafen.h"
#include "GZFileStream.h"

using namespace Mednafen;

void core_log(const char *fmt, ...);

GZFileStream::GZFileStream(const std::string& path, const MODE mode, const int level) :
	OpenedMode(mode), path_humesc(MDFN_strhumesc(path))
{
	throw MDFN_Error(ENOENT, "File does not exist");
}

GZFileStream::~GZFileStream()
{
}

void GZFileStream::close(void)
{
	core_log("*** GZFileStream::close\n");
}

uint64 GZFileStream::read(void *data, uint64 count, bool error_on_eof)
{
	core_log("*** GZFileStream::read\n");

	return 0;
}

int GZFileStream::get_line(std::string &str)
{
	core_log("*** GZFileStream::get_line\n");

	return 0;
}

void GZFileStream::flush(void)
{
	core_log("*** GZFileStream::flush\n");
}

void GZFileStream::write(const void *data, uint64 count)
{
	core_log("*** GZFileStream::write\n");
}

void GZFileStream::truncate(uint64 length)
{
	core_log("*** GZFileStream::truncate\n");
}

void GZFileStream::seek(int64 offset, int whence)
{
	core_log("*** GZFileStream::seek\n");
}

uint64 GZFileStream::size(void)
{
	core_log("*** GZFileStream::size\n");

	return 0;
}

uint64 GZFileStream::tell(void)
{
	core_log("*** GZFileStream::tell\n");

	return 0;
}

uint64 GZFileStream::attributes(void)
{
	core_log("*** GZFileStream::attributes\n");

	return 0;
}
