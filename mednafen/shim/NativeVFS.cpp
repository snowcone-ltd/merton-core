#include <mednafen/mednafen.h>
#include <mednafen/NativeVFS.h>
#include <mednafen/FileStream.h>

namespace Mednafen
{

NativeVFS::NativeVFS() : VirtualFS(MDFN_PS, (MDFN_PSS_STYLE == 2) ? "\\/" : MDFN_PSS)
{
}

NativeVFS::~NativeVFS()
{
}

Stream* NativeVFS::open(const std::string& path, const uint32 mode, const int do_lock, const bool throw_on_noent, const CanaryType canary)
{
	return NULL;
}

int NativeVFS::mkdir(const std::string& path, const bool throw_on_exist, const bool throw_on_noent)
{
	return 0;
}

bool NativeVFS::unlink(const std::string& path, const bool throw_on_noent, const CanaryType canary)
{
	return false;
}

void NativeVFS::rename(const std::string& oldpath, const std::string& newpath, const CanaryType canary)
{
}

bool NativeVFS::finfo(const std::string& path, FileInfo* fi, const bool throw_on_noent)
{
	return false;
}

void NativeVFS::readdirentries(const std::string& path, std::function<bool(const std::string&)> callb)
{
}

bool NativeVFS::is_absolute_path(const std::string& path)
{
	return false;
}

bool NativeVFS::is_driverel_path(const std::string& path)
{
	return false;
}

void NativeVFS::check_firop_safe(const std::string& path)
{
}

void NativeVFS::get_file_path_components(const std::string &file_path, std::string* dir_path_out, std::string* file_base_out, std::string *file_ext_out)
{
}

std::string NativeVFS::get_human_path(const std::string& path)
{
	return "";
}

}
