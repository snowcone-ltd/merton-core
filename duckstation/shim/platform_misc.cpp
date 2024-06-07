#include "util/platform_misc.h"

#include "../../core.h"

void core_log(const char *fmt, ...);

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

size_t PlatformMisc::GetRuntimePageSize()
{
	core_log("*** PlatformMisc::GetRuntimePageSize\n");

	return 0;
}

size_t PlatformMisc::GetRuntimeCacheLineSize()
{
	// XXX This gets called, may be important on ARM / Unix / macOS

	core_log("*** PlatformMisc::GetRuntimeCacheLineSize\n");

	return 0;
}
