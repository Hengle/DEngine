#include "DSystem.h"
#include "DOpenGLSystem.h"
#include "D3DSystem.h"

DSystem::DSystem()
{
	//m_applicationName = 0;
	//m_title = 0;
	m_timeMgr = 0;
	m_graphicsMgr = 0;
	m_logMgr = 0;
	m_inputMgr = 0;
	m_sceneMgr = 0;
	m_res = 0;
	//m_hInstance = 0;
	//m_hwnd = 0;
	//m_window = 0;
}

DSystem::~DSystem()
{
}

bool DSystem::Init(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api)
{
	System = this;
	//int width = D_DEFAULT_WIDTH, height = D_DEFAULT_HEIGHT;
	if (!OnInit(screenWidth, screenHeight, fullScreen, api))
		return false;

	m_timeMgr = new DTime();
	m_timeMgr->Init();

	m_sceneMgr = new DSceneManager();
	m_logMgr = new DLog();
	m_logMgr->Init();

	m_res = new DRes();
	m_res->Init("../Res/ResManifest.dres");

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

	OnShutdown();

	DShader::ReleaseGlobalConstants();
	m_res = NULL;

	System = NULL;
}

bool DSystem::Loop()
{
	if (m_inputMgr != NULL && !m_inputMgr->InputLoop())
	{
		return false;
	}
	if (!m_graphicsMgr->Frame())
	{
		return false;
	}
	return true;
}

DSystem * DSystem::Create(DGraphicsAPI api)
{
#ifdef _DGAPI_D3D9
	if (api == DGRAPHICS_API_D3D9)
		return new D3DSystem;
#endif
#ifdef _DGAPI_D3D10
	if (api == DGRAPHICS_API_D3D10)
		return new D3DSystem;
#endif
#ifdef _DGAPI_D3D11
	if (api == DGRAPHICS_API_D3D11)
		return new D3DSystem;
#endif
#ifdef _DGAPI_OPENGL
	if (api == DGRAPHICS_API_OPENGL)
		return new DOpenGLSystem;
#endif
	return nullptr;
}

//HWND DSystem::GetHWND()
//{
//	return System->m_hwnd;
//}

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

DSystem * DSystem::GetInstance()
{
	return System;
}
