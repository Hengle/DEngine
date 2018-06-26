#include "DGraphics.h"
#include "DSystem.h"
#include "D3D11Core.h"
#include "D3D10Core.h"
#include "D3D9Core.h"
#include "DImGUICore11.h"
#include "DImGUICore10.h"
#include "DImGUICore9.h"

DGraphics::DGraphics()
{
	m_GL = 0;
	m_GUI = 0;
}


DGraphics::~DGraphics()
{
}

bool DGraphics::Init(int width, int height, bool fullScreen, HWND hwnd, DGraphicsAPI api)
{
	m_API = api;
	if (api == DGRAPHICS_API_D3D11)
	{
		D3D11Core* gl = new D3D11Core();
		if (!gl->Init(width, height, fullScreen, hwnd))
		{
			return false;
		}
		m_GUI = new DImGUICore11();
		((DImGUICore11*)m_GUI)->Init(hwnd, gl->GetDevice(), gl->GetDeviceContext());
		m_GL = gl;
	}
	else if (api == DGRAPHICS_API_D3D10)
	{
		D3D10Core* gl = new D3D10Core();
		if (!gl->Init(width, height, fullScreen, hwnd))
		{
			return false;
		}
		m_GUI = new DImGUICore10();
		((DImGUICore10*)m_GUI)->Init(hwnd, gl->GetDevice());
		m_GL = gl;
	}
	else if (api == DGRAPHICS_API_D3D9)
	{
		D3D9Core* gl = new D3D9Core();
		if (!gl->Init(width, height, fullScreen, hwnd))
		{
			return false;
		}
		m_GUI = new DImGUICore9();
		((DImGUICore9*)m_GUI)->Init(hwnd, gl->GetDevice());
		m_GL = gl;
	}

	return true;
}

bool DGraphics::Render()
{
	DTime* time = DSystem::GetTimeMgr();
	DSceneManager* sceneManager = DSystem::GetSceneMgr();
	DLog* logManager = DSystem::GetLogMgr();

	time->Update();

	m_GUI->NewFrame();

	sceneManager->DrawGUI();

	logManager->DrawMsgs();

	m_GUI->EndFrame();

	while (time->BeginFixedUpdateLoop())
	{
		sceneManager->FixedUpdateScene();
	}

	sceneManager->UpdateScene();

	m_GL->BeginRender();

	sceneManager->RenderScene();

	m_GUI->Render();
	m_GL->EndRender();

	time->Wait();

	return true;
}

void DGraphics::Shutdown()
{
	if (m_GUI != NULL)
	{
		m_GUI->ShutDown();
		delete m_GUI;
	}
	m_GUI = NULL;

	if (m_GL != NULL)
	{
		m_GL->Destroy();
		delete m_GL;
	}
	m_GL = NULL;
	if (m_screenPlane != NULL)
	{
		m_screenPlane->Destroy();
		delete m_screenPlane;
		m_screenPlane = NULL;
	}
}
DGLCore * DGraphics::GetGLCore()
{
	return m_GL;
}

DImGUICore * DGraphics::GetGUICore()
{
	return m_GUI;
}

LRESULT CALLBACK DGraphics::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	if (m_GUI != NULL && m_GUI->GUIMessageHandler(hwnd, uMsg, wparam, lparam))
		return true;
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

void DGraphics::Clear(bool clearDepth, bool clearStencil, DColor & color)
{
	DSystem::GetGraphicsMgr()->GetGLCore()->Clear(clearDepth, clearStencil, color);
}

void DGraphics::ClearRenderTarget(DRenderTexture * res, bool clearDepth, bool clearStencil, DColor & color)
{
	if (res == NULL)
		return;
	DSystem::GetGraphicsMgr()->GetGLCore()->ClearRenderTarget(res->GetTextureRes(), clearDepth, clearStencil, color);
}

void DGraphics::SetDefaultRenderTarget()
{
	DSystem::GetGraphicsMgr()->GetGLCore()->SetDefaultRenderTarget();
}

void DGraphics::SetRenderTarget(DRenderTexture * res)
{
	if (res == NULL)
		return;
	DSystem::GetGraphicsMgr()->GetGLCore()->SetRenderTarget(res->GetTextureRes());
}

void DGraphics::DrawMesh(const DMesh * mesh, const DMatrix4x4 & matrix, DMaterial * material, const DCamera * camera)
{
	if (mesh == NULL || material == NULL || camera == NULL)
		return;
	DMatrix4x4 view, proj;
	camera->GetViewMatrix(view);
	camera->GetProjection(proj);

	/*DVector3 f, u, pos, e;
	DQuaterion rot;
	camera->GetTransform()->GetForward(f);
	camera->GetTransform()->GetUp(u);
	camera->GetTransform()->GetPosition(pos);
	camera->GetTransform()->GetEuler(e);
	camera->GetTransform()->GetRotation(rot);*/

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

	material->Apply();

	mesh->Draw();
	
}

void DGraphics::DrawTexture(DTexture * texture, DMaterial * material)
{
	if (DSystem::GetGraphicsMgr()->m_screenPlane == NULL)
	{
		DSystem::GetGraphicsMgr()->InitScreenPlane();
	}
	material->SetZWrite(false);
	material->SetZTest(DRSCompareFunc_Always);
	DMatrix4x4 world, proj;
	DMatrix4x4::Identity(&world);
	DMatrix4x4 view = DMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 1.0f);
	float screenWidth, screenHeight;
	DSystem::GetGraphicsMgr()->GetGLCore()->GetResolution(screenWidth, screenHeight);
	DMatrix4x4::Ortho(&proj, screenWidth, screenHeight, -100.0f, 100.0f);

	material->SetMatrix("worldMatrix", world);
	material->SetMatrix("viewMatrix", view);
	material->SetMatrix("projectionMatrix", proj);
	material->SetTexture("screenTexture", texture);

	material->Apply();

	DSystem::GetGraphicsMgr()->m_screenPlane->Draw();
}

void DGraphics::SetCullMode(DCullMode cullMode)
{
	DRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetCullMode(cullMode);
	}
}

void DGraphics::SetZWriteEnable(bool zwrite)
{
	DRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetZWriteEnable(zwrite);
	}
}

void DGraphics::SetZTestFunc(DRSCompareFunc ztest)
{
	DRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetZTestFunc(ztest);
	}
}

void DGraphics::GlBegin()
{
}

void DGraphics::GlEnd()
{
}

void DGraphics::Vector3(DVector3 &)
{
}

void DGraphics::Color(DColor &)
{
}

void DGraphics::InitScreenPlane()
{
	DMeshBufferDesc desc;

	int dataSize = sizeof(float) * 5;
	int vertexCount = 4;
	int indexCount = 6;
	int bufferLength = 5;

	float* buffer;
	unsigned long* indexBuffer;

	buffer = new float[vertexCount * 5];
	indexBuffer = new unsigned long[indexCount];

	float screenWidth, screenHeight;
	m_GL->GetResolution(screenWidth, screenHeight);

	buffer[0] = -0.5f*screenWidth; buffer[1] = -0.5f*screenHeight; buffer[2] = 0.0f; buffer[3] = 0.0f; buffer[4] = 1.0f;
	buffer[5] = -0.5f*screenWidth; buffer[6] = 0.5f*screenHeight; buffer[7] = 0.0f; buffer[8] = 0.0f; buffer[9] = 0.0f;
	buffer[10] = 0.5f*screenWidth; buffer[11] = 0.5f*screenHeight; buffer[12] = 0.0f; buffer[13] = 1.0f; buffer[14] = 0.0f;
	buffer[15] = 0.5f*screenWidth; buffer[16] = -0.5f*screenHeight; buffer[17] = 0.0f; buffer[18] = 1.0f; buffer[19] = 1.0f;

	indexBuffer[0] = 0; indexBuffer[1] = 1; indexBuffer[2] = 2;
	indexBuffer[3] = 0; indexBuffer[4] = 2; indexBuffer[5] = 3;

	desc.dataSize = dataSize;
	desc.dataCount = bufferLength;
	desc.indexCount = indexCount;
	desc.vertexCount = vertexCount;
	desc.indices = indexBuffer;
	desc.vertices = buffer;

	m_screenPlane = DMesh::Create(&desc);
}

