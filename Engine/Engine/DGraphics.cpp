#include "DGraphics.h"
#include "DSystem.h"

DGraphics::DGraphics()
{
	m_D3D = 0;
	//m_GUI = 0;
}


DGraphics::~DGraphics()
{
}

bool DGraphics::Init(int width, int height, bool fullScreen, HWND hwnd, DGraphicsAPI api)
{
	m_API = api;
	m_D3D = new D3D11Core();
	if (!m_D3D->Init(width, height, fullScreen, hwnd))
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

	m_D3D->BeginRender(0, 0, 1, 0.5f);

	sceneManager->RenderScene();

	//m_GUI->Render();
	m_D3D->EndRender();

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

	if (m_D3D != NULL)
	{
		m_D3D->Destroy();
		delete m_D3D;
	}
	m_D3D = NULL;

}
DGLCore * DGraphics::GetGLCore()
{
	return m_D3D;
}
//
//ID3D11Device * DGraphics::GetDevice()
//{
//	return m_D3D->GetDevice();
//}
//
//ID3D11DeviceContext * DGraphics::GetDeviceContext()
//{
//	return m_D3D->GetDeviceContext();
//}

LRESULT CALLBACK DGraphics::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	//if (m_GUI != NULL && m_GUI->GUIMessageHandler(hwnd, uMsg, wparam, lparam))
	//	return true;
	return false;
}

void DGraphics::GetResolution(FLOAT & width, FLOAT & height)
{
	m_D3D->GetResolution(width, height);
}

DGraphicsAPI DGraphics::GetAPI()
{
	return m_API;
}

//ID3D11Device * DGraphics::GetDevice()
//{
//	return m_D3D->GetDevice();
//}
//
//ID3D11DeviceContext * DGraphics::GetDeviceContext()
//{
//	return m_D3D->GetDeviceContext();
//}

void DGraphics::DrawMesh(const DMesh * mesh, const DMatrix4x4 & matrix, DMaterial * material, const DCamera * camera)
{
	DSystem::GetGraphicsMgr()->GetGLCore()->DrawMesh(mesh->GetBuffer(), mesh->GetDataSize());
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
	DVector3 cpos;
	world.Transpose();
	v.Transpose();
	p.Transpose();

	camera->GetTransform()->GetPosition(cpos);

	//MatrixBufferType bf;
	//bf.world = world;
	//bf.view = v;
	//bf.projection = p;

	//ViewBufferType vf;
	//vf.camPos = cpos;
	//vf.power = 1.3f;
	//vf.color = DColor(1.0f, 0.0f, 0.0f, 1.0f);

	material->SetMatrix("worldMatrix", world);
	material->SetMatrix("viewMatrix", v);
	material->SetMatrix("projectionMatrix", p);

	material->SetVector3("camPos", cpos);

//	material->SetCBuffer<MatrixBufferType>(bname, 0, bf);
	//material->SetCBuffer<ViewBufferType>(cname, 0, vf);

	int ccount = material->GetParamCount();
	int i;
	int pcount, poffset, pindex, stype;
	float* params;
	for (i = 0; i < ccount; i++)
	{
		material->GetParams(i, pcount, pindex, poffset, stype, &params);
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplyShaderParams(material->GetShader()->GetShaderBuffer(), pindex, poffset, pcount, stype, params);
	}

	DSystem::GetGraphicsMgr()->GetGLCore()->DrawShader(material->GetShader()->GetShaderBuffer(), mesh->GetIndexCount());
}

