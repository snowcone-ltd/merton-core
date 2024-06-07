#include "util/input_manager.h"

void InputManager::SetPadVibrationIntensity(u32 pad_index, float large_or_single_motor_intensity, float small_motor_intensity)
{
}

std::pair<float, float> InputManager::GetPointerAbsolutePosition(u32 index)
{
	return std::make_pair(0, 0);
}

const char *InputManager::InputSourceToString(InputSourceType clazz)
{
	return "";
}

GenericInputBindingMapping InputManager::GetGenericBindingMapping(std::string_view device)
{
	return {};
}

bool InputManager::MapController(SettingsInterface& si, u32 controller,
	const std::vector<std::pair<GenericInputBinding, std::string>>& mapping)
{
	return true;
}

void InputManager::ReloadBindings(SettingsInterface& si, SettingsInterface& binding_si, SettingsInterface& hotkey_binding_si)
{
}

void InputManager::ReloadSources(SettingsInterface& si, std::unique_lock<std::mutex>& settings_lock)
{
}

void InputManager::CloseSources()
{
}

void InputManager::PollSources()
{
}

void InputManager::PauseVibration()
{
}
