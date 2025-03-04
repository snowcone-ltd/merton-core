#include "mednafen.h"
#include "NativeVFS.h"
#include "FileStream.h"

using namespace Mednafen;

void core_log(const char *fmt, ...);

NativeVFS::NativeVFS() : VirtualFS(MDFN_PS, (MDFN_PSS_STYLE == 2) ? "\\/" : MDFN_PSS)
{
}

NativeVFS::~NativeVFS()
{
}

Stream* NativeVFS::open(const std::string& path, const uint32 mode, const int do_lock, const bool throw_on_noent, const CanaryType canary)
{
	return new FileStream(path, mode, do_lock);
}

int NativeVFS::mkdir(const std::string& path, const bool throw_on_exist, const bool throw_on_noent)
{
	return 1;
}

bool NativeVFS::finfo(const std::string& path, FileInfo* fi, const bool throw_on_noent)
{
	struct stat st;
	if (stat(path.c_str(), &st) != 0) {
		if (errno == ENOENT && !throw_on_noent)
		   return false;

		throw MDFN_Error(errno, "'stat' failed with errno %d", errno);
		return false;
	}

	if (fi) {
		FileInfo new_fi;

		new_fi.mtime_us = (int64) st.st_mtime * 1000 * 1000;
		new_fi.size = st.st_size;
		new_fi.is_regular = (st.st_mode & S_IFMT) == S_IFREG;
		new_fi.is_directory = (st.st_mode & S_IFMT) == S_IFDIR;

		*fi = new_fi;
	}

	return true;
}

bool NativeVFS::is_absolute_path(const std::string& path)
{
	if (!path.size())
		return false;

	if (is_path_separator(path[0]))
		return true;

	#if defined(WIN32) || defined(DOS)
		if (path.size() >= 2 && MDFN_isaz(path[0]) && path[1] == ':')
		return true;
	#endif

	return false;
}

bool NativeVFS::is_driverel_path(const std::string& path)
{
	#if defined(WIN32) || defined(DOS)
		if (path.size() >= 2 && MDFN_isaz(path[0]) && path[1] == ':' && (path.size() < 3 || !is_path_separator(path[2])))
			return true;
	#endif

	return false;
}

void NativeVFS::check_firop_safe(const std::string& path)
{
}

void NativeVFS::get_file_path_components(const std::string &file_path, std::string* dir_path_out, std::string* file_base_out, std::string *file_ext_out)
{
	#if defined(WIN32) || defined(DOS)
	if (file_path.size() >= 3 && MDFN_isaz(file_path[0]) &&
		file_path[1] == ':' && file_path.find_last_of(allowed_path_separators) == std::string::npos)
	{
		VirtualFS::get_file_path_components(file_path.substr(0, 2) + '.' + preferred_path_separator + file_path.substr(2), dir_path_out, file_base_out, file_ext_out);
		return;
	}
	#endif

	VirtualFS::get_file_path_components(file_path, dir_path_out, file_base_out, file_ext_out);
}

std::string NativeVFS::get_human_path(const std::string& path)
{
	return MDFN_sprintf(_("\"%s\""), MDFN_strhumesc(path).c_str());
}


// Unimplemented

bool NativeVFS::unlink(const std::string& path, const bool throw_on_noent, const CanaryType canary)
{
	core_log("*** NativeVFS::unlink\n");

	return true;
}

void NativeVFS::rename(const std::string& oldpath, const std::string& newpath, const CanaryType canary)
{
	core_log("*** NativeVFS::rename\n");
}

void NativeVFS::readdirentries(const std::string& path, std::function<bool(const std::string&)> callb)
{
	core_log("*** NativeVFS::readdirentries\n");
}
