#pragma once
#include "DGraphics.h"
#include <Windows.h>

class DImGUIBase
{

};

class DGUI
{
public:
	DGUI();
	~DGUI();
	void Init(HWND);
	void ShutDown();
	void NewFrame();

	void BeginWindow(const char* windowName);
	void EndWindow();
	void Label(const char* text);

	void Render();
	LRESULT CALLBACK GUIMessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	DImGUIBase* m_ImGUI;
};

