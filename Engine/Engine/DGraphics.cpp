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

bool DGraphics::Execute()
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

	//m_GL->BeginRender();

	sceneManager->RenderScene();

	m_GUI->Render();
	//m_GL->EndRender();
	m_GL->Present();

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
	if (m_skyMesh != NULL)
	{
		m_skyMesh->Destroy();
		delete m_skyMesh;
		m_skyMesh = NULL;
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

	DMatrix4x4 world = matrix;
	DMatrix4x4 v = view;
	DMatrix4x4 p = proj;

	material->SetMatrix("g_worldMatrix", world);
	material->SetMatrix("g_viewMatrix", v);
	material->SetMatrix("g_projectionMatrix", p);

	int passcount = material->GetPassCount();
	int i;
	for (i = 0; i < passcount; i++)
	{
		material->SetPass(i);
		mesh->Draw(material->GetVertexUsage(i));
	}

	
	
}

void DGraphics::DrawTexture(DTexture * texture, DMaterial * material)
{
	if (DSystem::GetGraphicsMgr()->m_screenPlane == NULL)
	{
		DSystem::GetGraphicsMgr()->InitScreenPlane();
	}
	DMatrix4x4 world, proj;
	DMatrix4x4::Identity(&world);
	DMatrix4x4 view = DMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 1.0f);
	float screenWidth, screenHeight;
	DSystem::GetGraphicsMgr()->GetGLCore()->GetResolution(screenWidth, screenHeight);
	DMatrix4x4::Ortho(&proj, screenWidth, screenHeight, -100.0f, 100.0f);

	material->SetMatrix("g_worldMatrix", world);
	material->SetMatrix("g_viewMatrix", view);
	material->SetMatrix("g_projectionMatrix", proj);
	material->SetTexture("screenTexture", texture);

	int passcount = material->GetPassCount();
	int i;

	for (i = 0; i < passcount; i++)
	{
		material->SetPass(i);
		DSystem::GetGraphicsMgr()->m_screenPlane->Draw(material->GetVertexUsage(i));
	}
}

void DGraphics::DrawSkyBox(DMaterial * material, const DCamera * camera)
{
	if (material == NULL || camera == NULL)
		return;
	if (DSystem::GetGraphicsMgr()->m_skyMesh == NULL)
	{
		DSystem::GetGraphicsMgr()->InitSkyBox();
	}
	DMatrix4x4 world;
	DTransform* transform = camera->GetTransform();
	DVector3 pos;
	transform->GetPosition(pos);
	DMatrix4x4::Translate(&world, pos.x, pos.y, pos.z);

	//material->SetZWrite(false);
	//material->SetCullMode(DCullMode_Off);

	DrawMesh(DSystem::GetGraphicsMgr()->m_skyMesh, world, material, camera);
}

void DGraphics::SetCullMode(DCullMode cullMode)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetCullMode(cullMode);
	}
}

void DGraphics::SetZWriteEnable(bool zwrite)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetZWriteEnable(zwrite);
	}
}

void DGraphics::SetZTestFunc(DRSCompareFunc ztest)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetZTestFunc(ztest);
	}
}

void DGraphics::SetBlendOp(DRSBlendOp op)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetBlendOp(op);
	}
}

void DGraphics::SetBlendEnable(bool enable)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetBlendEnable(enable);
	}
}

void DGraphics::SetBlendSrcFactor(DRSBlendFactor factor)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetBlendSrcFactor(factor);
	}
}

void DGraphics::SetBlendDstFactor(DRSBlendFactor factor)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetBlendDstFactor(factor);
	}
}

void DGraphics::SetViewPort(DRect & viewPort)
{
	DGLCore* gl = DSystem::GetGraphicsMgr()->GetGLCore();
	if (gl != NULL)
	{
		gl->SetViewPort(viewPort);
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

void DGraphics::GlVector3(DVector3 & vector)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlVector3(&vector);
	}
}

void DGraphics::GlVector(float x, float y, float z)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlVector(x, y, z);
	}
}

void DGraphics::GlColor(DColor & color)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlColor(&color);
	}
}

void DGraphics::GlPushMatrix()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlPushMatrix();
	}
}

void DGraphics::GlPopMatrix()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlPopMatrix();
	}
}

void DGraphics::GlLoadIndentity()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlLoadIdentity();
	}
}

void DGraphics::GlLoadProjectionMatrix(DMatrix4x4 & projection)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlLoadProjectionMatrix(projection);
	}
}

void DGraphics::GlLoadOrtho()
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlLoadOrtho();
	}
}

void DGraphics::GlMultiMatrix(DMatrix4x4 & matrix)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlMultiMatrix(matrix);
	}
}

void DGraphics::GetModelView(DMatrix4x4 & modelView)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GetModelView(modelView);
	}
}

void DGraphics::GetProjection(DMatrix4x4 & projection)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GetProjection(projection);
	}
}

void DGraphics::InitScreenPlane()
{
	//DMeshBufferDesc desc;

	//int dataSize = sizeof(float) * 5;
	int vertexCount = 4;
	int indexCount = 6;
	//int bufferLength = 5;

	float *vertices,*uvs;
	unsigned long *indexBuffer;

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

	m_screenPlane = new DMesh();
	m_screenPlane->SetVertices(vertices, 4);
	m_screenPlane->SetUVs(0, uvs, 4);
	m_screenPlane->SetIndices(indexBuffer, 6);

}

void DGraphics::InitSkyBox()
{
	m_skyMesh = new DMesh();
	float *vertices = new float[108];
	unsigned long* indices = new unsigned long[36];

	float sz = 500.0f;

	vertices[0] = -sz; vertices[1] = -sz; vertices[2] = -sz;
	vertices[3] = sz; vertices[4] = sz; vertices[5] = -sz;
	vertices[6] = -sz; vertices[7] = sz; vertices[8] = -sz;

	vertices[9] = -sz; vertices[10] = -sz; vertices[11] = -sz;
	vertices[12] = sz; vertices[13] = -sz; vertices[14] = -sz;
	vertices[15] = sz; vertices[16] = sz; vertices[17] = -sz;

	vertices[18] = -sz; vertices[19] = sz; vertices[20] = -sz;
	vertices[21] = sz; vertices[22] = sz; vertices[23] = sz;
	vertices[24] = -sz; vertices[25] = sz; vertices[26] = sz;

	vertices[27] = -sz; vertices[28] = sz; vertices[29] = -sz;
	vertices[30] = sz; vertices[31] = sz; vertices[32] = -sz;
	vertices[33] = sz; vertices[34] = sz; vertices[35] = sz;

	vertices[36] = -sz; vertices[37] = -sz; vertices[38] = sz;
	vertices[39] = -sz; vertices[40] = sz; vertices[41] = sz;
	vertices[42] = sz; vertices[43] = sz; vertices[44] = sz;

	vertices[45] = -sz; vertices[46] = -sz; vertices[47] = sz;
	vertices[48] = sz; vertices[49] = sz; vertices[50] = sz;
	vertices[51] = sz; vertices[52] = -sz; vertices[53] = sz;

	vertices[54] = -sz; vertices[55] = -sz; vertices[56] = -sz;
	vertices[57] = -sz; vertices[58] = -sz; vertices[59] = sz;
	vertices[60] = sz; vertices[61] = -sz; vertices[62] = sz;

	vertices[63] = -sz; vertices[64] = -sz; vertices[65] = -sz;
	vertices[66] = sz; vertices[67] = -sz; vertices[68] = sz;
	vertices[69] = sz; vertices[70] = -sz; vertices[71] = -sz;

	vertices[72] = sz; vertices[73] = -sz; vertices[74] = -sz;
	vertices[75] = sz; vertices[76] = sz; vertices[77] = sz;
	vertices[78] = sz; vertices[79] = sz; vertices[80] = -sz;

	vertices[81] = sz; vertices[82] = -sz; vertices[83] = -sz;
	vertices[84] = sz; vertices[85] = -sz; vertices[86] = sz;
	vertices[87] = sz; vertices[88] = sz; vertices[89] = sz;
	
	vertices[90] = -sz; vertices[91] = -sz; vertices[92] = -sz;
	vertices[93] = -sz; vertices[94] = sz; vertices[95] = -sz;
	vertices[96] = -sz; vertices[97] = sz; vertices[98] = sz;

	vertices[99] = -sz; vertices[100] = -sz; vertices[101] = -sz;
	vertices[102] = -sz; vertices[103] = sz; vertices[104] = sz;
	vertices[105] = -sz; vertices[106] = -sz; vertices[107] = sz;

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 4; indices[5] = 5;

	indices[6] = 6; indices[7] = 7; indices[8] = 8;
	indices[9] = 9; indices[10] = 10; indices[11] = 11;

	indices[12] = 12; indices[13] = 13; indices[14] = 14;
	indices[15] = 15; indices[16] = 16; indices[17] = 17;

	indices[18] = 18; indices[19] = 19; indices[20] = 20;
	indices[21] = 21; indices[22] = 22; indices[23] = 23;

	indices[24] = 24; indices[25] = 25; indices[26] = 26;
	indices[27] = 27; indices[28] = 28; indices[29] = 29;

	indices[30] = 30; indices[31] = 31; indices[32] = 32;
	indices[33] = 33; indices[34] = 34; indices[35] = 35;

	m_skyMesh->SetVertices(vertices, 36);
	m_skyMesh->SetIndices(indices, 36);
}

