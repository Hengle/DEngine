#include "DGUI.h"
#include "DSystem.h"
#include "imgui.h"

void DGUI::BeginWindow(const char * windowName)
{
	ImGui::Begin(windowName);
}

void DGUI::EndWindow()
{
	ImGui::End();
}

void DGUI::Label(const char * text, ...)
{
	va_list args;
	va_start(args, text);
	ImGui::TextV(text, args);
	va_end(args);
}

void DGUI::Label(const DColor & color, const char * text, ...)
{
	ImVec4 col;
	col.x = color.r;
	col.y = color.g;
	col.z = color.b;
	col.w = color.a;
	va_list args;
	va_start(args, text);
	ImGui::TextColoredV(col, text, args);
	va_end(args);
}

bool DGUI::IsGUIActive()
{
	return ImGui::IsAnyItemActive();
}
