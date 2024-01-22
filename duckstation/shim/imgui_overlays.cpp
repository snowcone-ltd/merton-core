#include "core/imgui_overlays.h"
#include "core/host.h"

void Host::DisplayLoadingScreen(const char* message, int progress_min, int progress_max,
	int progress_value)
{
}

void ImGuiManager::DestroyOverlayTextures()
{
}

void ImGuiManager::RenderTextOverlays()
{
}

void ImGuiManager::RenderDebugWindows()
{
}

void ImGuiManager::RenderOverlayWindows()
{
}

void SaveStateSelectorUI::SelectNextSlot(bool open_selector)
{
}

void SaveStateSelectorUI::SelectPreviousSlot(bool open_selector)
{
}

void SaveStateSelectorUI::LoadCurrentSlot()
{
}

void SaveStateSelectorUI::SaveCurrentSlot()
{
}

bool SaveStateSelectorUI::IsOpen()
{
	return false;
}

void SaveStateSelectorUI::RefreshList(const std::string& serial)
{
}

void SaveStateSelectorUI::Clear()
{
}

void SaveStateSelectorUI::ClearList()
{
}
