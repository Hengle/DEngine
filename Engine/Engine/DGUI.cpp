#include "DGUI.h"
#include "DSystem.h"
#include "imgui.h"

void DGUI::BeginWindow(const char * windowName)
{
	if (DSystem::GetGraphicsMgr()->GetGUICore() != NULL)
		ImGui::Begin(windowName);
}

void DGUI::EndWindow()
{
	if (DSystem::GetGraphicsMgr()->GetGUICore() != NULL)
		ImGui::End();
}

void DGUI::Label(const char * text, ...)
{
	if (DSystem::GetGraphicsMgr()->GetGUICore() == NULL)
		return;
	va_list args;
	va_start(args, text);
	ImGui::TextV(text, args);
	va_end(args);
}

void DGUI::Label(const DColor & color, const char * text, ...)
{
	if (DSystem::GetGraphicsMgr()->GetGUICore() == NULL)
		return;
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

bool DGUI::Button(const char * text, const DVector2 & size)
{
	if (DSystem::GetGraphicsMgr()->GetGUICore() == NULL)
		return false;
	ImVec2 vec;
	vec.x = size.x;
	vec.y = size.y;
	return ImGui::Button(text, vec);
}

bool DGUI::IsGUIActive()
{
	if (DSystem::GetGraphicsMgr()->GetGUICore() == NULL)
		return false;
	return ImGui::IsAnyItemActive();
}
