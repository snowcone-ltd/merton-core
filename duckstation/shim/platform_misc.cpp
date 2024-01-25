#include "util/platform_misc.h"

void PlatformMisc::SuspendScreensaver()
{
}

void PlatformMisc::ResumeScreensaver()
{
}

bool PlatformMisc::PlaySoundAsync(const char* path)
{
	return false;
}

std::optional<WindowInfo> Host::GetTopLevelWindowInfo()
{
	return std::nullopt;
}
