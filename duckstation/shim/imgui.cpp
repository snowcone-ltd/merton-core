#include "imgui/include/imgui.h"

bool ImGui::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
	return true;
}

void ImGui::End()
{
}

void ImGui::SetNextWindowSize(const ImVec2& size, ImGuiCond cond)
{
}

float ImGui::GetWindowWidth()
{
	return 0;
}

float ImGui::GetCursorPosX()
{
	return 0;
}

float ImGui::GetCursorPosY()
{
	return 0;
}

void ImGui::SetCursorPosX(float local_x)
{
}

void ImGui::SetCursorPosY(float local_y)
{
}

void ImGui::SameLine(float offset_from_start_x, float spacing)
{
}

void ImGui::NewLine()
{
}

void ImGui::TextUnformatted(const char* text, const char* text_end)
{
}

void ImGui::Text(const char* fmt, ...)
{
}

void ImGui::TextColored(const ImVec4& col, const char* fmt, ...)
{
}

bool ImGui::Button(const char* label, const ImVec2& size)
{
	return true;
}

bool ImGui::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags)
{
	return true;
}

void ImGui::Columns(int count, const char* id, bool border)
{
}

void ImGui::NextColumn()
{
}

void ImGui::SetColumnWidth(int column_index, float width)
{
}

void ImGui::Render()
{
}

void ImGui::PushStyleColor(ImGuiCol idx, const ImVec4& col)
{
}

void ImGui::PopStyleColor(int count)
{
}
