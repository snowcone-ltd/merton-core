#include "mednafen.h"
#include "PSFLoader.h"

using namespace Mednafen;

PSFTags::PSFTags()
{
}

PSFTags::~PSFTags()
{
}

void PSFTags::AddTag(char *tag_line)
{
}

void PSFTags::LoadTags(Stream* fp)
{
}

int64 PSFTags::GetTagI(const char *name)
{
	return 0;
}

bool PSFTags::TagExists(const char *name)
{
	return false;
}

std::string PSFTags::GetTag(const char *name)
{
	return "";
}

void PSFTags::EraseTag(const char *name)
{
}

PSFLoader::PSFLoader()
{
}

PSFLoader::~PSFLoader()
{
}

bool PSFLoader::TestMagic(uint8 version, Stream* fp)
{
	return false;
}

PSFTags PSFLoader::LoadInternal(uint8 version, uint32 max_exe_size, VirtualFS* vfs, const std::string& dir_path, Stream *fp, uint32 level, bool force_ignore_pcsp)
{
	return {};
}

PSFTags PSFLoader::Load(uint8 version, uint32 max_exe_size, VirtualFS* vfs, const std::string& dir_path, Stream *fp)
{
	return {};
}

void PSFLoader::HandleReserved(Stream* fp, uint32 len)
{
}

void PSFLoader::HandleEXE(Stream* fp, bool ignore_pcsp)
{
}
