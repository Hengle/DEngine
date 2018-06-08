#include "DGraphics.h"
#include "DSystem.h"
#include "D3D10Core.h"
#include "D3D11Core.h"
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

	m_GL->BeginRender(0, 0, 1, 0.5f);

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
	m_GL->GetResolution(width, height);
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
	/*world.Transpose();
	v.Transpose();
	p.Transpose();*/

	

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

	/*D3D9Core* core = (D3D9Core*)DSystem::GetGraphicsMgr()->GetGLCore();

	DShaderBuffer9* buffer = (DShaderBuffer9*)material->GetShader()->GetShaderBuffer();

	buffer->SetMatrix("worldMatrix", core->GetDevice(), world);
	buffer->SetMatrix("viewMatrix", core->GetDevice(), v);
	buffer->SetMatrix("projectionMatrix", core->GetDevice(), p);*/

	//material->SetVector3("camPos", cpos);

//	material->SetCBuffer<MatrixBufferType>(bname, 0, bf);
	//material->SetCBuffer<ViewBufferType>(cname, 0, vf);

	

	int ccount = material->GetParamCount();
	int i;
	int pcount, poffset, pindex, psize, stype;
	float* params;
	for (i = 0; i < ccount; i++)
	{
		material->GetParams(i, pcount, pindex, poffset, psize, stype, &params);
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplyShaderParams(material->GetShader()->GetShaderBuffer(), pindex, poffset, psize, stype, params);
	}

	DSystem::GetGraphicsMgr()->GetGLCore()->DrawShader(material->GetShader()->GetShaderBuffer(), mesh->GetIndexCount());

	
	//core->GetDevice()->SetTransform(D3DTS_VIEW)

	//D3DXMATRIX worldmatrix = D3DXMATRIX(matrix.m00, matrix.m01, matrix.m02, matrix.m03,
	//	matrix.m10, matrix.m11, matrix.m12, matrix.m13,
	//	matrix.m20, matrix.m21, matrix.m22, matrix.m23,
	//	matrix.m30, matrix.m31, matrix.m32, matrix.m33);
	//D3DXMATRIX viewmatrix = D3DXMATRIX(view.m00, view.m01, view.m02, view.m03,
	//	view.m10, view.m11, view.m12, view.m13,
	//	view.m20, view.m21, view.m22, view.m23,
	//	view.m30, view.m31, view.m32, view.m33);
	//D3DXMATRIX projmatrix = D3DXMATRIX(proj.m00, proj.m01, proj.m02, proj.m03,
	//	proj.m10, proj.m11, proj.m12, proj.m13,
	//	proj.m20, proj.m21, proj.m22, proj.m23,
	//	proj.m30, proj.m31, proj.m32, proj.m33);

	///*D3DXMatrixTranspose(&worldmatrix, &worldmatrix);
	//D3DXMatrixTranspose(&viewmatrix, &viewmatrix);
	//D3DXMatrixTranspose(&projmatrix, &projmatrix);*/

	//core->GetDevice()->SetTransform(D3DTS_WORLD, &worldmatrix);
	//core->GetDevice()->SetTransform(D3DTS_VIEW, &viewmatrix);
	//core->GetDevice()->SetTransform(D3DTS_PROJECTION, &projmatrix);
	

	DSystem::GetGraphicsMgr()->GetGLCore()->DrawMesh(mesh->GetBuffer(), mesh->GetDataSize());
}

