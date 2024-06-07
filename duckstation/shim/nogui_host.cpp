#include "core/host.h"
#include "core/system.h"
#include "common/log.h"
#include "util/gpu_device.h"

void core_log(const char *fmt, ...);

const char *g_scm_tag_str = "";

void Host::ReportErrorAsync(std::string_view title, std::string_view message)
{
	core_log("%s\n", std::string(message).c_str());
}

void Host::ReportDebuggerMessage(std::string_view message)
{
	core_log("%s\n", std::string(message).c_str());
}

void Host::ReportFatalError(std::string_view title, std::string_view message)
{
	core_log("Fatal: %s\n", std::string(message).c_str());
}

std::optional<std::string> Host::ReadResourceFileToString(std::string_view filename, bool allow_override)
{
	return std::nullopt;
}

std::optional<std::time_t> Host::GetResourceFileTimestamp(std::string_view filename, bool allow_override)
{
	return std::nullopt;
}

bool Host::ResourceFileExists(std::string_view filename, bool allow_override)
{
	return false;
}

std::optional<std::vector<u8>> Host::ReadResourceFile(std::string_view filename, bool allow_override)
{
	return std::nullopt;
}

s32 Host::Internal::GetTranslatedStringImpl(std::string_view context,
	std::string_view msg, char* tbuf, size_t tbuf_space)
{
	int32_t n = snprintf(tbuf, tbuf_space, "%s", std::string(msg).c_str());

	return n > 0 ? n : 0;
}

std::string Host::TranslatePluralToString(const char* context, const char* msg, const char* disambiguation, int count)
{
	return "";
}

void Host::FrameDone()
{
}

bool Host::ConfirmMessage(std::string_view title, std::string_view message)
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

void Host::ReleaseRenderWindow()
{
}
