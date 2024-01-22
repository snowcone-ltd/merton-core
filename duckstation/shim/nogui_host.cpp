#include "core/host.h"
#include "core/system.h"
#include "common/log.h"
#include "util/gpu_device.h"

void core_log(const char *fmt, ...);

const char *g_scm_tag_str = "";

void Host::ReportErrorAsync(const std::string_view& title, const std::string_view& message)
{
	core_log("%s\n", std::string(message).c_str());
}

void Host::ReportDebuggerMessage(const std::string_view& message)
{
	core_log("%s\n", std::string(message).c_str());
}

std::optional<std::string> Host::ReadResourceFileToString(std::string_view filename, bool allow_override)
{
	core_log("READ RESOURCE: %s\n", std::string(filename).c_str());

	return "";
}

std::optional<std::time_t> Host::GetResourceFileTimestamp(std::string_view filename, bool allow_override)
{
	core_log("TS RESOURCE: %s\n", std::string(filename).c_str());

	return 0;
}

bool Host::ConfirmMessage(const std::string_view& title, const std::string_view& message)
{
	core_log("CONFIRM\n");
	return false;
}

void Host::RunOnCPUThread(std::function<void()> function, bool block)
{
	core_log("RUN ON CPU THREAD\n");
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
	core_log("RESOURCE EXISTS: %s\n", std::string(filename).c_str());

	return false;
}

std::optional<std::vector<u8>> Host::ReadResourceFile(std::string_view filename, bool allow_override)
{
	core_log("READF RESOURCE: %s\n", std::string(filename).c_str());

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
	core_log("PUMP\n");
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
