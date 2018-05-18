#pragma once
#include "imgui_impl_dx11.h"
#include "DSceneManager.h"
#include <Windows.h>

class DImGUI
{
public:
	DImGUI();
	~DImGUI();
	void Init(HWND, ID3D11Device*, ID3D11DeviceContext*);
	void ShutDown();
	void NewFrame();

	void BeginWindow(const char* windowName);
	void EndWindow();
	void Label(const char* text);

	//void DrawGUI(DSceneManager*);
	void Render();
	LRESULT CALLBACK GUIMessageHandler(HWND, UINT, WPARAM, LPARAM);
};

