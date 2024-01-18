#include "Utilities/FolderUtilities.h"
#include "Utilities/UTF8Util.h"

#if __has_include(<filesystem>)
	#include <filesystem>
	namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#endif

string FolderUtilities::_firmwareFolderOverride = "";

void FolderUtilities::SetHomeFolder(string homeFolder)
{
}

string FolderUtilities::GetHomeFolder()
{
	return "";
}

void FolderUtilities::AddKnownGameFolder(string gameFolder)
{
}

vector<string> FolderUtilities::GetKnownGameFolders()
{
	return vector<string>();
}

void FolderUtilities::SetFolderOverrides(string saveFolder, string saveStateFolder, string screenshotFolder, string firmwareFolder)
{
	_firmwareFolderOverride = firmwareFolder;
}

string FolderUtilities::GetSaveFolder()
{
	return "";
}

string FolderUtilities::GetFirmwareFolder()
{
	return _firmwareFolderOverride;
}

string FolderUtilities::GetHdPackFolder()
{
	return "";
}

string FolderUtilities::GetDebuggerFolder()
{
	return "";
}

string FolderUtilities::GetSaveStateFolder()
{
	return "";
}

string FolderUtilities::GetScreenshotFolder()
{
	return "";
}

string FolderUtilities::GetRecentGamesFolder()
{
	return "";
}

string FolderUtilities::GetExtension(string filename)
{
	size_t position = filename.find_last_of('.');

	if (position != string::npos) {
		string ext = filename.substr(position, filename.size() - position);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		return ext;
	}

	return "";
}

void FolderUtilities::CreateFolder(string folder)
{
}

vector<string> FolderUtilities::GetFolders(string rootFolder)
{
	return vector<string>();
}

vector<string> FolderUtilities::GetFilesInFolder(string rootFolder, std::unordered_set<string> extensions, bool recursive)
{
	return vector<string>();
}

string FolderUtilities::GetFilename(string filepath, bool includeExtension)
{
	fs::path filename = fs::u8path(filepath).filename();

	if (!includeExtension)
		filename.replace_extension("");

	return filename.u8string();
}

string FolderUtilities::GetFolderName(string filepath)
{
	return fs::u8path(filepath).remove_filename().u8string();
}

string FolderUtilities::CombinePath(string folder, string filename)
{
	//Windows supports forward slashes for paths, too.  And fs::u8path is abnormally slow.
	if (folder[folder.length() - 1] != '/') {
		return folder + "/" + filename;

	} else {
		return folder + filename;
	}
}
