#include "mednafen.h"

using namespace Mednafen;

void core_log(const char *fmt, ...);

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

VirtualFS* Mednafen::MDFN_OpenArchive(VirtualFS* vfs, const std::string& path, const std::vector<FileExtensionSpecStruct>& known_ext, std::string* path_out)
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

void Mednafen::MDFN_BackupSavFile(const uint8 max_backup_count, const char* sav_ext)
{
}
