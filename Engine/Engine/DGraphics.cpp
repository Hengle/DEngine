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
	m_glDrawer = 0;
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

	m_glDrawer = new DGLDrawer();

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
	if (m_glDrawer != NULL)
	{
		m_glDrawer->Release();
		delete m_glDrawer;
		m_glDrawer = NULL;
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

void DGraphics::BeginScene(bool clearDepth, bool clearStencil, DColor & color, DRenderTexture * renderTexture)
{
	if (renderTexture != NULL)
	{
		
		SetRenderTarget(renderTexture);
		Clear(clearDepth, clearStencil, color, renderTexture);
	}
	else
	{
		SetRenderTarget();
		Clear(clearDepth, clearStencil, color);
	}
}

void DGraphics::EndScene(DRenderTexture * renderTexture)
{
	if (DSystem::GetGraphicsMgr()->m_glDrawer != NULL)
	{
		DSystem::GetGraphicsMgr()->m_glDrawer->PostGL();
	}
	if (renderTexture != NULL)
	{
		EndSetRenderTarget(renderTexture);
	}
	else
	{
		EndSetRenderTarget();
	}
}

void DGraphics::Clear(bool clearDepth, bool clearStencil, DColor & color, DRenderTexture * renderTexture)
{
	if (renderTexture != NULL)
	{
		DRenderTextureViewRes* res = renderTexture->GetTextureRes();
		if(res != NULL)
			DSystem::GetGraphicsMgr()->GetGLCore()->Clear(clearDepth, clearStencil, color, res);
	}
	else
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->Clear(clearDepth, clearStencil, color);
	}
}

void DGraphics::SetRenderTarget(DRenderTexture * res)
{
	if (res != NULL)
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->SetRenderTarget(res->GetTextureRes());
	}
	else
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->SetRenderTarget();
	}
}

void DGraphics::EndSetRenderTarget(DRenderTexture * res)
{
	if (res != NULL)
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->EndSetRenderTarget(res->GetTextureRes());
	}
	else
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->EndSetRenderTarget();
	}
}

void DGraphics::DrawMesh(DMesh * mesh, const DMatrix4x4 & matrix, DMaterial * material, const DCamera * camera)
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

	mesh->Draw(material->GetVertexUsage());
	
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

	DSystem::GetGraphicsMgr()->m_screenPlane->Draw(material->GetVertexUsage());
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
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlBegin();
	}
}

void DGraphics::GlEnd()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlEnd();
	}
}

void DGraphics::GLVector3(DVector3 & vector)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlVector3(&vector);
	}
}

void DGraphics::GLVector(float x, float y, float z)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlVector(x, y, z);
	}
}

void DGraphics::GLColor(DColor & color)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlColor(&color);
	}
}

void DGraphics::GLPushMatrix()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlPushMatrix();
	}
}

void DGraphics::GLPopMatrix()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlPopMatrix();
	}
}

void DGraphics::GLLoadIndentity()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlLoadIdentity();
	}
}

void DGraphics::InitScreenPlane()
{
	//DMeshBufferDesc desc;

	//int dataSize = sizeof(float) * 5;
	int vertexCount = 4;
	int indexCount = 6;
	//int bufferLength = 5;

	float* vertices,*uvs;
	unsigned long* indexBuffer;

	vertices = new float[vertexCount * 3];
	uvs = new float[vertexCount * 2];
	indexBuffer = new unsigned long[indexCount];

	float screenWidth, screenHeight;
	m_GL->GetResolution(screenWidth, screenHeight);

	vertices[0] = -0.5f*screenWidth; vertices[1] = -0.5f*screenHeight; vertices[2] = 0.0f;
	vertices[3] = -0.5f*screenWidth; vertices[4] = 0.5f*screenHeight; vertices[5] = 0.0f;
	vertices[6] = 0.5f*screenWidth; vertices[7] = 0.5f*screenHeight; vertices[8] = 0.0f;
	vertices[9] = 0.5f*screenWidth; vertices[10] = -0.5f*screenHeight; vertices[11] = 0.0f;

	uvs[0] = 0.0f; uvs[1] = 1.0f;
	uvs[2] = 0.0f; uvs[3] = 0.0f;
	uvs[4] = 1.0f; uvs[5] = 0.0f;
	uvs[6] = 1.0f; uvs[7] = 1.0f;

	indexBuffer[0] = 0; indexBuffer[1] = 1; indexBuffer[2] = 2;
	indexBuffer[3] = 0; indexBuffer[4] = 2; indexBuffer[5] = 3;

	//desc.dataSize = dataSize;
	//desc.dataCount = bufferLength;
	/*desc.indexCount = indexCount;
	desc.vertexCount = vertexCount;
	desc.indices = indexBuffer;
	desc.vertices = vertices;
	desc.colors = 0;
	desc.normals = 0;
	desc.uv2s = 0;
	desc.uv3s = 0;
	desc.uvs = uvs*/

	//m_screenPlane = DMesh::Create(&desc);
	m_screenPlane = new DMesh();
	m_screenPlane->SetVertices(vertices, 4);
	m_screenPlane->SetUVs(0, uvs, 4);
	m_screenPlane->SetIndices(indexBuffer, 6);
}

