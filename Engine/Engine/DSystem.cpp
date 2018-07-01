#include "DSystem.h"

DSystem::DSystem()
{
	m_applicationName = 0;
	m_title = 0;
	m_timeMgr = 0;
	m_graphicsMgr = 0;
	m_logMgr = 0;
	m_inputMgr = 0;
	m_sceneMgr = 0;
	m_res = 0;
	m_hInstance = 0;
	m_hwnd = 0;
}

DSystem::~DSystem()
{
}

bool DSystem::Init()
{
	int width = D_DEFAULT_WIDTH, height = D_DEFAULT_HEIGHT;
	InitWindow(width, height, false);

	m_res = new DRes();
	m_res->Init();

	m_inputMgr = new DInput();
	if (!m_inputMgr->Init(m_hInstance, m_hwnd, width, height))
	{
		return false;
	}

	m_timeMgr = new DTime();
	m_timeMgr->Init();

	m_graphicsMgr = new DGraphics();
	if (!m_graphicsMgr->Init(width, height, false, m_hwnd, DGRAPHICS_API_D3D11))
	{
		return false;
	}

	m_sceneMgr = new DSceneManager();
	m_logMgr = new DLog();
	m_logMgr->Init();

	return true;
}

bool DSystem::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	bool done = false;
	int result = 0;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Loop();
			if (!result)
			{
				done = true;
			}
		}
	}

	return false;
}

void DSystem::Shutdown()
{
	DestroyWindow(m_hwnd);

	if (m_inputMgr != NULL)
	{
		m_inputMgr->Shutdown();
		delete m_inputMgr;
	}
	m_inputMgr = NULL;

	if (m_timeMgr != NULL)
	{
		m_timeMgr->Shutdown();
		delete m_timeMgr;
	}
	m_timeMgr = NULL;

	if (m_graphicsMgr != NULL)
	{
		m_graphicsMgr->Shutdown();
		delete m_graphicsMgr;
	}
	m_graphicsMgr = NULL;

	if (m_sceneMgr != NULL)
	{
		m_sceneMgr->UnloadAllScene();
		delete m_sceneMgr;
	}
	m_sceneMgr = NULL;

	if (m_logMgr != NULL)
	{
		m_logMgr->Shutdown();
		delete m_logMgr;
	}
	m_logMgr = NULL;
	if (m_res != NULL)
	{
		m_res->Shutdown();
		delete m_res;
	}
	DShader::ReleaseGlobalConstants();
	m_res = NULL;

	m_hwnd = NULL;
	m_applicationName = NULL;

	m_hInstance = NULL;

	System = NULL;
}

void DSystem::InitWindow(int& width, int& height, bool fullScreen)
{
	m_hInstance = GetModuleHandle(NULL);
	m_applicationName = L"DEngine";
	m_title = L"DEngine";

	System = this;

	WNDCLASSEX wnd;
	wnd.cbSize = sizeof(wnd);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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

bool DSystem::Loop()
{
	if (!m_inputMgr->InputLoop(m_hwnd))
	{
		return false;
	}
	if (!m_graphicsMgr->Render())
	{
		return false;
	}
	return true;
}

LRESULT CALLBACK DSystem::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

HWND DSystem::GetHWND()
{
	return System->m_hwnd;
}

DSceneManager * DSystem::GetSceneMgr()
{
	return System->m_sceneMgr;
}

DGraphics * DSystem::GetGraphicsMgr()
{
	return System->m_graphicsMgr;
}

DLog * DSystem::GetLogMgr()
{
	return System->m_logMgr;
}

DTime * DSystem::GetTimeMgr()
{
	return System->m_timeMgr;
}

DInput * DSystem::GetInputMgr()
{
	return System->m_inputMgr;
}

DRes * DSystem::GetResMgr()
{
	return System->m_res;
}

void DSystem::Quit()
{
}

LRESULT CALLBACK SysWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return System->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}