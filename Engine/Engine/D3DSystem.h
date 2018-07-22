#pragma once
#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11

#include "DSystem.h"

class D3DSystem : public DSystem
{
public:
	D3DSystem();
	~D3DSystem();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

protected:
	virtual bool OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api);
	virtual void OnShutdown();

private:
	void InitWindow(int&, int&, bool);

private:
	HINSTANCE m_hInstance;
	HWND m_hwnd;
	LPCWSTR m_applicationName;
	LPCWSTR m_title;
};

static LRESULT CALLBACK SysWndProc(HWND, UINT, WPARAM, LPARAM);

#endif