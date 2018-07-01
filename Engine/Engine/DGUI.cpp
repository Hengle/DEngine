#include "DGUI.h"
#include "DSystem.h"
#include "imgui.h"

void DGUI::BeginWindow(const char * windowName)
{
}

void DGUI::EndWindow()
{
}

void DGUI::Label(const char * text, ...)
{
	va_list args;
	va_start(args, text);
	ImGui::TextV(text, args);
	va_end(args);
}

bool DGUI::IsGUIActive()
{
	return ImGui::IsAnyItemActive();
}
