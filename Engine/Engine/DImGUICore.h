#pragma once
#include <Windows.h>
#include "imgui.h"

class DImGUICore
{
public:
	virtual void ShutDown() = 0;
	virtual void NewFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Render() = 0;
	LRESULT CALLBACK GUIMessageHandler(HWND, UINT, WPARAM, LPARAM);

protected:
	static bool ImGui_ImplWin32_UpdateMouseCursor();

private:
	IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

