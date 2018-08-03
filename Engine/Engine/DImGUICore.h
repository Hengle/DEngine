#pragma once
#include <Windows.h>
#include "imgui.h"
#include "DEngineDefine.h"

interface IImGUICore
{
public:
	virtual void ShutDown() = 0;
	virtual void NewFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Render() = 0;
	virtual LRESULT CALLBACK GUIMessageHandler(HWND, UINT, WPARAM, LPARAM) = 0;
};

