#include "util/imgui_manager.h"

float Host::GetOSDScale()
{
	return 1;
}

void Host::AddOSDMessage(std::string message, float duration)
{
}

void Host::AddKeyedOSDMessage(std::string key, std::string message, float duration)
{
}

void Host::ClearOSDMessages()
{
}

void Host::AddIconOSDMessage(std::string key, const char* icon, std::string message, float duration)
{
}

void Host::RemoveKeyedOSDMessage(std::string key)
{
}

float ImGuiManager::GetWindowWidth()
{
	return 1920;
}

float ImGuiManager::GetWindowHeight()
{
	return 1080;
}

void ImGuiManager::SetSoftwareCursor(u32 index, std::string image_path, float image_scale, u32 multiply_color)
{
}

void ImGuiManager::ClearSoftwareCursor(u32 index)
{
}

void ImGuiManager::SetSoftwareCursorPosition(u32 index, float pos_x, float pos_y)
{
}

bool ImGuiManager::Initialize(float global_scale, bool show_osd_messages, Error* error)
{
	return true;
}

void ImGuiManager::Shutdown()
{
}

void ImGuiManager::WindowResized()
{
}

void ImGuiManager::SetGlobalScale(float global_scale)
{
}

void ImGuiManager::SetShowOSDMessages(bool enable)
{
}

void ImGuiManager::NewFrame()
{
}

void ImGuiManager::RenderOSDMessages()
{
}

void ImGuiManager::RenderSoftwareCursors()
{
}
