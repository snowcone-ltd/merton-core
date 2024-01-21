#include "core/achievements.h"
#include "core/controller.h"
#include "core/fullscreen_ui.h"
#include "core/game_list.h"
#include "core/gpu.h"
#include "core/host.h"
#include "core/imgui_overlays.h"
#include "core/settings.h"
#include "core/system.h"

#include "util/gpu_device.h"
#include "util/imgui_manager.h"
#include "util/ini_settings_interface.h"
#include "util/input_manager.h"
#include "util/platform_misc.h"

#include "common/assert.h"
#include "common/byte_stream.h"
#include "common/crash_handler.h"
#include "common/file_system.h"
#include "common/log.h"
#include "common/path.h"
#include "common/string_util.h"
#include "common/threading.h"

#include <cinttypes>
#include <cmath>
#include <condition_variable>
#include <csignal>
#include <thread>

const HotkeyInfo g_host_hotkeys[1] = {};
const char *g_scm_tag_str = "";

void Host::ReportErrorAsync(const std::string_view& title, const std::string_view& message)
{
}

bool Host::ConfirmMessage(const std::string_view& title, const std::string_view& message)
{
	return false;
}

void Host::ReportDebuggerMessage(const std::string_view& message)
{
}

std::span<const std::pair<const char*, const char*>> Host::GetAvailableLanguageList()
{
	return {};
}

bool Host::ChangeLanguage(const char* new_language)
{
	return false;
}

void Host::AddFixedInputBindings(SettingsInterface& si)
{
}

void Host::OnInputDeviceConnected(const std::string_view& identifier, const std::string_view& device_name)
{
}

void Host::OnInputDeviceDisconnected(const std::string_view& identifier)
{
}

s32 Host::Internal::GetTranslatedStringImpl(const std::string_view& context, const std::string_view& msg, char* tbuf,
                                            size_t tbuf_space)
{
	return 0;
}

bool Host::ResourceFileExists(std::string_view filename, bool allow_override)
{
	return false;
}

std::optional<std::vector<u8>> Host::ReadResourceFile(std::string_view filename, bool allow_override)
{
	return std::vector<u8>();
}

std::optional<std::string> Host::ReadResourceFileToString(std::string_view filename, bool allow_override)
{
	return "";
}

std::optional<std::time_t> Host::GetResourceFileTimestamp(std::string_view filename, bool allow_override)
{
	return 0;
}

void Host::LoadSettings(SettingsInterface& si, std::unique_lock<std::mutex>& lock)
{
}

void Host::CheckForSettingsChanges(const Settings& old_settings)
{
}

void Host::CommitBaseSettingChanges()
{
}

std::optional<WindowInfo> Host::AcquireRenderWindow(bool recreate_window)
{
	return {};
}

void Host::ReleaseRenderWindow()
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

void Host::BeginPresentFrame()
{
}

void Host::RequestResizeHostDisplay(s32 width, s32 height)
{
}

void Host::OpenURL(const std::string_view& url)
{
}

bool Host::CopyTextToClipboard(const std::string_view& text)
{
	return false;
}

void Host::OnPerformanceCountersUpdated()
{
}

void Host::OnGameChanged(const std::string& disc_path, const std::string& game_serial, const std::string& game_name)
{
}

void Host::OnAchievementsLoginRequested(Achievements::LoginRequestReason reason)
{
}

void Host::OnAchievementsLoginSuccess(const char* username, u32 points, u32 sc_points, u32 unread_messages)
{
}

void Host::OnAchievementsRefreshed()
{
}

void Host::OnAchievementsHardcoreModeChanged(bool enabled)
{
}

void Host::SetMouseMode(bool relative, bool hide_cursor)
{
}

void Host::PumpMessagesOnCPUThread()
{
}

void Host::RunOnCPUThread(std::function<void()> function, bool block /* = false */)
{
}

void Host::RefreshGameListAsync(bool invalidate_cache)
{
}

void Host::CancelGameListRefresh()
{
}

bool Host::IsFullscreen()
{
	return false;
}

void Host::SetFullscreen(bool enabled)
{
}

std::optional<WindowInfo> Host::GetTopLevelWindowInfo()
{
	return {};
}

void Host::RequestExit(bool allow_confirm)
{
}

void Host::RequestSystemShutdown(bool allow_confirm, bool save_state)
{
}

std::optional<u32> InputManager::ConvertHostKeyboardStringToCode(const std::string_view& str)
{
	return 0;
}

std::optional<std::string> InputManager::ConvertHostKeyboardCodeToString(u32 code)
{
	return "";
}

const char* InputManager::ConvertHostKeyboardCodeToIcon(u32 code)
{
	return "";
}
