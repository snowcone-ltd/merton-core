#include "mednafen.h"
#include <mednafen/FileStream.h>
#include <mednafen/compress/GZFileStream.h>
#include <mednafen/compress/ZIPReader.h>
#include <mednafen/compress/ZLInflateFilter.h>
#include <mednafen/compress/ZstdDecompressFilter.h>
#include <mednafen/MemoryStream.h>
#include <mednafen/IPSPatcher.h>
#include <mednafen/string/string.h>

#include <trio/trio.h>

#include "file.h"
#include "general.h"

void core_log(const char *fmt, ...);

namespace Mednafen
{

void MDFNFILE::ApplyIPS(Stream *ips)
{
	core_log("*** MDFNFILE::ApplyIPS\n");
}

MDFNFILE::MDFNFILE(VirtualFS* vfs, const std::string& path, const std::vector<FileExtensionSpecStruct>& known_ext, const char *purpose, int* monocomp_double_ext)
{
	core_log("*** MDFNFILE::MDFNFILE:0\n");
}

MDFNFILE::MDFNFILE(VirtualFS* vfs, const std::string& path, const char *purpose, int* monocomp_double_ext)
{
	core_log("*** MDFNFILE::MDFNFILE:1\n");
}

MDFNFILE::~MDFNFILE()
{
	core_log("*** MDFNFILE::~MDFNFILE\n");
}

VirtualFS* MDFN_OpenArchive(VirtualFS* vfs, const std::string& path, const std::vector<FileExtensionSpecStruct>& known_ext, std::string* path_out)
{
	return NULL;
}

void MDFNFILE::Open(VirtualFS* vfs, const std::string& path, const char *purpose, int* monocomp_double_ext)
{
	core_log("*** MDFNFILE::Open\n");
}

void MDFNFILE::Close(void) noexcept
{
	core_log("*** MDFNFILE::Close\n");
}

bool MDFN_DumpToFile(const std::string& path, const std::vector<PtrLengthPair> &pearpairs, bool throw_on_error)
{
	core_log("*** MDFN_DumpToFile:0\n");

	return false;
}

bool MDFN_DumpToFile(const std::string& path, const void *data, uint64 length, bool throw_on_error)
{
	core_log("*** MDFN_DumpToFile:1\n");

	return false;
}

std::unique_ptr<Stream> MDFN_AmbigGZOpenHelper(const std::string& path, std::vector<size_t> good_sizes)
{
	core_log("*** MDFN_AmbigGZOpenHelper\n");

	return {};
}

void MDFN_BackupSavFile(const uint8 max_backup_count, const char* sav_ext)
{
}

}
