#pragma once

static class DGUI
{
public:
	static void BeginWindow(const char* windowName);
	static void EndWindow();
	static void Label(const char* text, ...);

	static bool IsGUIActive();
};

