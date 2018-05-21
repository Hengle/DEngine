#include "DGraphics.h"


DGraphics::DGraphics()
{
	m_D3D = 0;
	m_GUI = 0;
	//m_line = 0;
}


DGraphics::~DGraphics()
{
}

bool DGraphics::Init(int width, int height, bool fullScreen, HWND hwnd)
{
	m_D3D = new D3DCore();
	if(!m_D3D->Init(width, height, fullScreen, hwnd))
	{	
		return false;
	}
	m_GUI = new DImGUI();
	m_GUI->Init(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());

	//m_line = new DLine();
	//m_line->Init(m_D3D->GetDevice());

	return true;
}

bool DGraphics::Render(DSceneManager* sceneManager, DLogManager* logManager, DTime* time)
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

void DGraphics::Shutdown()
{
	/*if (m_line != NULL)
	{
		m_line->Destroy();
		delete m_line;
	}
	m_line = NULL;*/
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

ID3D11Device * DGraphics::GetDevice()
{
	return m_D3D->GetDevice();
}

ID3D11DeviceContext * DGraphics::GetDeviceContext()
{
	return m_D3D->GetDeviceContext();
}

LRESULT CALLBACK DGraphics::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	if (m_GUI != NULL && m_GUI->GUIMessageHandler(hwnd, uMsg, wparam, lparam))
		return true;
	return false;
}

void DGraphics::GetResolution(FLOAT & width, FLOAT & height)
{
	m_D3D->GetResolution(width, height);
}

//void DGraphics::DrawLine(const D3DXVECTOR3& begin, const D3DXVECTOR3& end)
//{
//	m_line->UpdateVertex(begin, end, m_D3D->GetDeviceContext());
//	m_line->Render(m_D3D->GetDeviceContext());
//}
