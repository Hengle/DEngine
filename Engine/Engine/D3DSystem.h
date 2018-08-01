#pragma once
#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11

#include "DSystem.h"
#include "DirectInputCore.h"

class D3DSystem : public DSystem
{
public:
	D3DSystem();
	~D3DSystem();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	HWND GetHWND();

protected:
	virtual bool OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api);
	virtual void OnShutdown();
	virtual void OnFrameBegin();
	virtual IInputCore* GetInputCore();

private:
	void InitWindow(int&, int&, bool);

private:
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	LPCWSTR m_applicationName;
	LPCWSTR m_title;
	DirectInputCore* m_inputCore;
};

static LRESULT CALLBACK SysWndProc(HWND, UINT, WPARAM, LPARAM);

#endif