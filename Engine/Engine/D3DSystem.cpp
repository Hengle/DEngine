#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11

#include "D3DSystem.h"


D3DSystem::D3DSystem() : DSystem()
{
	m_applicationName = 0;
	m_title = 0;
	m_hInstance = 0;
	m_hwnd = 0;
}


D3DSystem::~D3DSystem()
{
}

LRESULT D3DSystem::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_graphicsMgr != NULL && m_graphicsMgr->MessageHandler(hwnd, uMsg, wParam, lParam))
		return true;
	switch (uMsg)
	{
	default:
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

bool D3DSystem::OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api)
{
	InitWindow(screenWidth, screenHeight, fullScreen);

	m_inputMgr = new DInput();
	if (!m_inputMgr->Init(m_hInstance, m_hwnd, screenWidth, screenHeight))
	{
		return false;
	}

	m_graphicsMgr = new DGraphics();
	if (!m_graphicsMgr->Init(screenWidth, screenHeight, fullScreen, m_hwnd, api))
	{
		return false;
	}
}

void D3DSystem::OnShutdown()
{
	DestroyWindow(m_hwnd);

	m_hwnd = NULL;
	m_applicationName = NULL;

	m_hInstance = NULL;
}

void D3DSystem::InitWindow(int & width, int & height, bool)
{
	m_hInstance = GetModuleHandle(NULL);
	m_applicationName = L"DEngine";
	m_title = L"DEngine";

	WNDCLASSEX wnd;
	wnd.cbSize = sizeof(wnd);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wnd.hIconSm = wnd.hIcon;
	wnd.hInstance = m_hInstance;
	wnd.lpfnWndProc = SysWndProc;
	wnd.lpszClassName = m_applicationName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wnd);

	int posX, posY;
	posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_title, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, posX, posY, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetFocus(m_hwnd);
}

LRESULT CALLBACK SysWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
	
		D3DSystem* system = (D3DSystem*)DSystem::GetInstance();
		return system->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}

#endif