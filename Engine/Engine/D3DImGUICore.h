#pragma once
#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11
#include "DImGUICore.h"

class D3DImGUICore : IImGUICore
{
public:
	D3DImGUICore();
	~D3DImGUICore();

	virtual LRESULT CALLBACK GUIMessageHandler(HWND, UINT, WPARAM, LPARAM);

protected:
	static bool ImGui_ImplWin32_UpdateMouseCursor();

private:
	IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif