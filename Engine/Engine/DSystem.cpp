#include "DSystem.h"
#include <dwmapi.h>

DSystem::DSystem()
{
	m_applicationName = 0;
	m_title = 0;
	m_time = 0;
	m_graphics = 0;
	m_logManager = 0;
	m_input = 0;
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

	DInput::CreateInstance(&m_input);
	if (!m_input->Init(m_hInstance, m_hwnd, width, height))
	{
		return false;
	}

	m_time = new DTime();
	m_time->Init();

	m_graphics = new DGraphicsCore();
	if (!m_graphics->Init(width, height, false, m_hwnd, DGRAPHICS_API_D3D11))
	{
		return false;
	}

	m_sceneManager = new DSceneManager();
	m_logManager = new DLogManager();
	m_logManager->Init();

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

	if (m_input != NULL) 
	{
		m_input->Shutdown();
		delete m_input;
	}
	m_input = NULL;

	m_time->Shutdown();
	delete m_time;
	m_time = NULL;

	if (m_graphics != NULL)
	{
		m_graphics->Shutdown();
		delete m_graphics;
	}
	m_graphics = NULL;

	m_sceneManager->UnloadAllScene();
	delete m_sceneManager;
	m_sceneManager = NULL;

	m_logManager->Shutdown();
	delete m_logManager;
	m_logManager = NULL;

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
	if (!m_input->InputLoop(m_hwnd))
	{
		return false;
	}
	if (!m_graphics->Render(m_sceneManager, m_logManager, m_time)) 
	{
		return false;
	}
	return true;
}

LRESULT CALLBACK DSystem::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_graphics != NULL && m_graphics->MessageHandler(hwnd, uMsg, wParam, lParam))
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

DSceneManager * DSystem::GetSceneManager()
{
	return System->m_sceneManager;
}

DGraphicsCore * DSystem::GetGraphicsCore()
{
	return System->m_graphics;
}

DLogManager * DSystem::GetLogManager()
{
	return System->m_logManager;
}

DTime * DSystem::GetTime()
{
	return System->m_time;
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