#include "core/host.h"
#include "core/system.h"
#include "util/gpu_device.h"

const char *g_scm_tag_str = "";

void Host::ReportErrorAsync(const std::string_view& title, const std::string_view& message)
{
}

std::optional<std::string> Host::ReadResourceFileToString(std::string_view filename, bool allow_override)
{
	return "";
}

std::optional<std::time_t> Host::GetResourceFileTimestamp(std::string_view filename, bool allow_override)
{
	return 0;
}

void Host::ReportDebuggerMessage(const std::string_view& message)
{
}

bool Host::ConfirmMessage(const std::string_view& title, const std::string_view& message)
{
	return false;
}

void Host::RunOnCPUThread(std::function<void()> function, bool block)
{
}

void Host::RequestSystemShutdown(bool allow_confirm, bool save_state)
{
}

bool Host::IsFullscreen()
{
	return false;
}

void Host::SetFullscreen(bool enabled)
{
}

bool Host::ResourceFileExists(std::string_view filename, bool allow_override)
{
	return false;
}

std::optional<std::vector<u8>> Host::ReadResourceFile(std::string_view filename, bool allow_override)
{
	return std::vector<u8>();
}

s32 Host::Internal::GetTranslatedStringImpl(const std::string_view& context,
	const std::string_view& msg, char* tbuf, size_t tbuf_space)
{
	return 0;
}

void Host::LoadSettings(SettingsInterface& si, std::unique_lock<std::mutex>& lock)
{
}

void Host::CheckForSettingsChanges(const Settings& old_settings)
{
}

void Host::OnSystemStarting()
{
}

void Host::OnSystemStarted()
{
}

void Host::OnSystemPaused()
{
}

void Host::OnSystemResumed()
{
}

void Host::OnSystemDestroyed()
{
}

void Host::OnIdleStateChanged()
{
}

void Host::OnPerformanceCountersUpdated()
{
}

void Host::OnGameChanged(const std::string& disc_path, const std::string& game_serial, const std::string& game_name)
{
}

void Host::PumpMessagesOnCPUThread()
{
}

void Host::RequestResizeHostDisplay(s32 width, s32 height)
{
}

void Host::BeginPresentFrame()
{
}

void Host::ReleaseRenderWindow()
{
}
