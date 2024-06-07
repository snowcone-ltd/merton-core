#include "achievements.h"

bool Achievements::IsHardcoreModeActive()
{
	return false;
}

bool Achievements::Initialize()
{
	return false;
}

void Achievements::UpdateSettings(const Settings& old_config)
{
}

void Achievements::ResetClient()
{
}

bool Achievements::ConfirmSystemReset()
{
	return true;
}

bool Achievements::Shutdown(bool allow_cancel)
{
	return true;
}

void Achievements::OnSystemPaused(bool paused)
{
}

void Achievements::FrameUpdate()
{
}

void Achievements::IdleUpdate()
{
}

bool Achievements::DoState(StateWrapper& sw)
{
	return true;
}

void Achievements::GameChanged(const std::string& path, CDImage* image)
{
}

bool Achievements::ResetHardcoreMode(bool is_booting)
{
	return false;
}

void Achievements::DisableHardcoreMode()
{
}

bool Achievements::ConfirmHardcoreModeDisable(const char* trigger)
{
	return false;
}

bool Achievements::IsActive()
{
	return false;
}

void Achievements::ConfirmHardcoreModeDisableAsync(const char* trigger, std::function<void(bool)> callback)
{
}
