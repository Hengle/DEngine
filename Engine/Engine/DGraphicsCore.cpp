#include "DGraphicsCore.h"


DGraphicsCore::DGraphicsCore()
{
	m_D3D = 0;
	m_GUI = 0;
}


DGraphicsCore::~DGraphicsCore()
{
}

bool DGraphicsCore::Init(int width, int height, bool fullScreen, HWND hwnd, DGraphicsAPI api)
{
	m_D3D = new D3DCore11();
	if(!m_D3D->Init(width, height, fullScreen, hwnd))
	{	
		return false;
	}
	m_GUI = new DImGUI();
	m_GUI->Init(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());

	return true;
}

bool DGraphicsCore::Render(DSceneManager* sceneManager, DLogManager* logManager, DTime* time)
{
	time->Update();

	m_GUI->NewFrame();

	sceneManager->DrawGUI();

	logManager->DrawMsgs();

	while (time->BeginFixedUpdateLoop())
	{
		sceneManager->FixedUpdateScene();
	}

	sceneManager->UpdateScene();

	m_D3D->BeginRender(0, 0, 1, 0.5f);

	sceneManager->RenderScene(m_D3D->GetDeviceContext());

	m_GUI->Render();
	m_D3D->EndRender();

	time->Wait();

	return true;
}

void DGraphicsCore::Shutdown()
{
	if (m_GUI != NULL)
	{
		m_GUI->ShutDown();
		delete m_GUI;
	}
	m_GUI = NULL;

	if (m_D3D != NULL)
	{
		m_D3D->Destroy();
		delete m_D3D;
	}
	m_D3D = NULL;
	
}

ID3D11Device * DGraphicsCore::GetDevice()
{
	return m_D3D->GetDevice();
}

ID3D11DeviceContext * DGraphicsCore::GetDeviceContext()
{
	return m_D3D->GetDeviceContext();
}

LRESULT CALLBACK DGraphicsCore::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	if (m_GUI != NULL && m_GUI->GUIMessageHandler(hwnd, uMsg, wparam, lparam))
		return true;
	return false;
}

void DGraphicsCore::GetResolution(FLOAT & width, FLOAT & height)
{
	m_D3D->GetResolution(width, height);
}

