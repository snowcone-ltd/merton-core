#include "achievements.h"

namespace Achievements {

static std::recursive_mutex mutex;
static std::string game_title = "";
static std::string rich_presence = "";

std::unique_lock<std::recursive_mutex> GetLock()
{
	return std::unique_lock(mutex);
}

rc_client_t* GetClient()
{
	return NULL;
}

bool Initialize()
{
	return false;
}

void UpdateSettings(const Settings& old_config)
{
}

void ResetClient()
{
}

bool ConfirmSystemReset()
{
	return true;
}

bool Shutdown(bool allow_cancel)
{
	return true;
}

void OnSystemPaused(bool paused)
{
}

void FrameUpdate()
{
}

void IdleUpdate()
{
}

bool NeedsIdleUpdate()
{
	return false;
}

bool DoState(StateWrapper& sw)
{
	return true;
}

bool Login(const char* username, const char* password, Error* error)
{
	return false;
}

void Logout()
{
}

void GameChanged(const std::string& path, CDImage* image)
{
}

bool ResetHardcoreMode()
{
	return false;
}

void DisableHardcoreMode()
{
}

bool ConfirmHardcoreModeDisable(const char* trigger)
{
	return false;
}

void ConfirmHardcoreModeDisableAsync(const char* trigger, std::function<void(bool)> callback)
{
}

bool IsHardcoreModeActive()
{
	return false;
}

bool IsUsingRAIntegration()
{
	return false;
}

bool IsActive()
{
	return false;
}

bool HasActiveGame()
{
	return false;
}

u32 GetGameID()
{
	return 0;
}

bool HasAchievementsOrLeaderboards()
{
	return false;
}

bool HasAchievements()
{
	return false;
}

bool HasLeaderboards()
{
	return false;
}

bool HasRichPresence()
{
	return false;
}

const std::string& GetRichPresenceString()
{
	return rich_presence;
}

const std::string& GetGameTitle()
{
	return game_title;
}

void ClearUIState()
{
}

void DrawGameOverlays()
{
}

void DrawPauseMenuOverlays()
{
}

#ifndef __ANDROID__

bool PrepareAchievementsWindow()
{
	return false;
}

void DrawAchievementsWindow()
{
}

bool PrepareLeaderboardsWindow()
{
	return false;
}

void DrawLeaderboardsWindow()
{
}

#endif // __ANDROID__

#ifdef ENABLE_RAINTEGRATION
void SwitchToRAIntegration()
{
}

namespace RAIntegration {

void MainWindowChanged(void* new_handle)
{
}

void GameChanged()
{
}

std::vector<std::tuple<int, std::string, bool>> GetMenuItems()
{
	return std::vector<std::tuple<int, std::string, bool>>();
}

void ActivateMenuItem(int item)
{
}

} // namespace RAIntegration
#endif
} // namespace Achievements
