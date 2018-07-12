#pragma once
#include "DColor.h"

/*GUI类*/
static class DGUI
{
public:
	static void BeginWindow(const char* windowName);
	static void EndWindow();
	static void Label(const char* text, ...);
	static void Label(const DColor&, const char* text, ...);

	/*当前gui是否处于激活状态*/
	static bool IsGUIActive();
};

