#pragma once
#include "DColor.h"
#include "DMath.h"

/*GUI类*/
static class DGUI
{
public:
	static void BeginWindow(const char* windowName);
	static void EndWindow();
	static void Label(const char* text, ...);
	static void Label(const DColor&, const char* text, ...);
	static bool Button(const char* text, const DVector2& size = DVector2(0.0f,0.0f));

	/*当前gui是否处于激活状态*/
	static bool IsGUIActive();
};

