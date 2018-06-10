#include "DGraphics.h"
#include "DSystem.h"
#include "D3D11Core.h"
#include "D3D10Core.h"
#include "D3D9Core.h"

DGraphics::DGraphics()
{
	m_GL = 0;
	//m_GUI = 0;
}


DGraphics::~DGraphics()
{
}

bool DGraphics::Init(int width, int height, bool fullScreen, HWND hwnd, DGraphicsAPI api)
{
	m_API = api;
	if (api == DGRAPHICS_API_D3D11)
		m_GL = new D3D11Core();
	else if (api == DGRAPHICS_API_D3D10)
		m_GL = new D3D10Core();
	else if (api == DGRAPHICS_API_D3D9)
		m_GL = new D3D9Core();
	if (!m_GL->Init(width, height, fullScreen, hwnd))
	{
		return false;
	}
	//m_GUI = new DImGUI();
	//m_GUI->Init(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());

	return true;
}

bool DGraphics::Render()
{
	DTime* time = DSystem::GetTimeMgr();
	DSceneManager* sceneManager = DSystem::GetSceneMgr();
	DLog* logManager = DSystem::GetLogMgr();

	time->Update();

	//m_GUI->NewFrame();

	sceneManager->DrawGUI();

	logManager->DrawMsgs();

	while (time->BeginFixedUpdateLoop())
	{
		sceneManager->FixedUpdateScene();
	}

	sceneManager->UpdateScene();

	m_GL->BeginRender();

	sceneManager->RenderScene();

	//m_GUI->Render();
	m_GL->EndRender();

	time->Wait();

	return true;
}

void DGraphics::Shutdown()
{
	//if (m_GUI != NULL)
	//{
	//	m_GUI->ShutDown();
	//	delete m_GUI;
	//}
	//m_GUI = NULL;

	if (m_GL != NULL)
	{
		m_GL->Destroy();
		delete m_GL;
	}
	m_GL = NULL;

}
DGLCore * DGraphics::GetGLCore()
{
	return m_GL;
}

LRESULT CALLBACK DGraphics::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	//if (m_GUI != NULL && m_GUI->GUIMessageHandler(hwnd, uMsg, wparam, lparam))
	//	return true;
	return false;
}

void DGraphics::GetResolution(float & width, float & height)
{
	m_GL->GetResolution(width, height);
}

DGraphicsAPI DGraphics::GetAPI()
{
	return m_API;
}

void DGraphics::DrawMesh(const DMesh * mesh, const DMatrix4x4 & matrix, DMaterial * material, const DCamera * camera)
{
	if (mesh == NULL || material == NULL || camera == NULL)
		return;
	DMatrix4x4 view, proj;
	camera->GetViewMatrix(view);
	camera->GetProjection(proj);

	DVector3 f, u, pos, e;
	DQuaterion rot;
	camera->GetTransform()->GetForward(f);
	camera->GetTransform()->GetUp(u);
	camera->GetTransform()->GetPosition(pos);
	camera->GetTransform()->GetEuler(e);
	camera->GetTransform()->GetRotation(rot);

	DMatrix4x4 world = matrix;
	DMatrix4x4 v = view;
	DMatrix4x4 p = proj;
	//DVector3 cpos;
	/*world.Transpose();
	v.Transpose();
	p.Transpose();*/

	

	//camera->GetTransform()->GetPosition(cpos);

	material->SetMatrix("worldMatrix", world);
	material->SetMatrix("viewMatrix", v);
	material->SetMatrix("projectionMatrix", p);

	material->Apply(mesh->GetIndexCount());
	mesh->Draw();
}

