#include "mednafen.h"
#include "PSFLoader.h"

void core_log(const char *fmt, ...);

namespace Mednafen
{

PSFTags::PSFTags()
{
	core_log("*** PSFLoader\n");
}

PSFTags::~PSFTags()
{
	core_log("*** PSFLoader\n");
}

void PSFTags::AddTag(char *tag_line)
{
	core_log("*** PSFLoader\n");
}

void PSFTags::LoadTags(Stream* fp)
{
	core_log("*** PSFLoader\n");
}

int64 PSFTags::GetTagI(const char *name)
{
	core_log("*** PSFLoader\n");
	return 0;
}

bool PSFTags::TagExists(const char *name)
{
	core_log("*** PSFLoader\n");
	return false;
}

std::string PSFTags::GetTag(const char *name)
{
	core_log("*** PSFLoader\n");
	return "";
}

void PSFTags::EraseTag(const char *name)
{
	core_log("*** PSFLoader\n");
}

PSFLoader::PSFLoader()
{
	core_log("*** PSFLoader\n");
}

PSFLoader::~PSFLoader()
{
	core_log("*** PSFLoader\n");
}

bool PSFLoader::TestMagic(uint8 version, Stream* fp)
{
	core_log("*** PSFLoader\n");
	return false;
}

PSFTags PSFLoader::LoadInternal(uint8 version, uint32 max_exe_size, VirtualFS* vfs, const std::string& dir_path, Stream *fp, uint32 level, bool force_ignore_pcsp)
{
	core_log("*** PSFLoader\n");
	return {};
}

PSFTags PSFLoader::Load(uint8 version, uint32 max_exe_size, VirtualFS* vfs, const std::string& dir_path, Stream *fp)
{
	core_log("*** PSFLoader\n");
	return {};
}

void PSFLoader::HandleReserved(Stream* fp, uint32 len)
{
	core_log("*** PSFLoader\n");
}

void PSFLoader::HandleEXE(Stream* fp, bool ignore_pcsp)
{
	core_log("*** PSFLoader\n");
}

}
