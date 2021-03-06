﻿#include "DGraphics.h"
#include "DSystem.h"
#include "D3D11Core.h"
#include "D3D10Core.h"
#include "D3D9Core.h"
#include "DOpenGLCore.h"
#include "DImGUICore11.h"
#include "DImGUICore10.h"
#include "DImGUICore9.h"
#include "DImGUICoreOpGL.h"
#include "D3DSystem.h"
#include "DOpenGLSystem.h"

DGraphics::DGraphics()
{
	m_GL = 0;
	m_GUI = 0;
	m_glDrawer = 0;
	m_globalRenderShader = 0;
	m_drawCall = 0;
	m_renderer = 0;
	m_activeMaterial = 0;
}


DGraphics::~DGraphics()
{
}

bool DGraphics::Init(int width, int height, bool fullScreen, DGraphicsAPI api)
{
	m_API = api;
#ifdef _DGAPI_D3D11
	if (api == DGRAPHICS_API_D3D11)
	{
		D3D11Core* gl = new D3D11Core();
		HWND hwnd = ((D3DSystem*)DSystem::GetInstance())->GetHWND();
		if (!gl->Init(width, height, fullScreen, hwnd))
		{
			return false;
		}
		m_GUI = (IImGUICore*)new DImGUICore11();
		((DImGUICore11*)m_GUI)->Init(hwnd, gl->GetDevice(), gl->GetDeviceContext());
		m_GL = gl;
	}
#endif
#ifdef _DGAPI_D3D10
	if (api == DGRAPHICS_API_D3D10)
	{
		D3D10Core* gl = new D3D10Core();
		HWND hwnd = ((D3DSystem*)DSystem::GetInstance())->GetHWND();
		if (!gl->Init(width, height, fullScreen, hwnd))
		{
			return false;
		}
		m_GUI = (IImGUICore*)new DImGUICore10();
		((DImGUICore10*)m_GUI)->Init(hwnd, gl->GetDevice());
		m_GL = gl;
	}
#endif
#ifdef _DGAPI_D3D9
	if (api == DGRAPHICS_API_D3D9)
	{
		D3D9Core* gl = new D3D9Core();
		HWND hwnd = ((D3DSystem*)DSystem::GetInstance())->GetHWND();
		if (!gl->Init(width, height, fullScreen, hwnd))
		{
			return false;
		}
		m_GUI = (IImGUICore*)new DImGUICore9();
		((DImGUICore9*)m_GUI)->Init(hwnd, gl->GetDevice());
		m_GL = gl;
	}
#endif
#ifdef _DGAPI_OPENGL
	if (api == DGRAPHICS_API_OPENGL)
	{
		DOpenGLCore* gl = new DOpenGLCore();
		GLFWwindow* window = ((DOpenGLSystem*)DSystem::GetInstance())->GetWindow();
		if (!gl->Init(width, height, fullScreen, window))
		{
			return false;
		}
		DImGUICoreOpGL* gui = new DImGUICoreOpGL();
		gui->Init(window, true);
		m_GUI = (IImGUICore*)gui;
		m_GL = gl;
	}
#endif

	if (m_GL == NULL)
		return false;

	m_renderer = new DRenderer();

	m_glDrawer = new DGLDrawer();

	return true;
}

bool DGraphics::Frame()
{

	DTime* time = DSystem::GetTimeMgr();
	DSceneManager* sceneManager = DSystem::GetSceneMgr();
	DLog* logManager = DSystem::GetLogMgr();

	time->Update();

	if(m_GUI != NULL)
		m_GUI->NewFrame();

	sceneManager->DrawGUI();

	logManager->DrawMsgs();

	if (m_GUI != NULL)
		m_GUI->EndFrame();

	while (time->BeginFixedUpdateLoop())
	{
		sceneManager->FixedUpdateScene();
	}

	sceneManager->UpdateScene();

	//m_GL->BeginRender();
	m_drawCall = 0;
	sceneManager->RenderScene();

	if (m_GUI != NULL)
		m_GUI->Render();
	//m_GL->EndRender();

	DScene::DrawShadow();

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
	if (m_skyGeometry != NULL)
	{
		m_skyGeometry->Destroy();
		delete m_skyGeometry;
		m_skyGeometry = NULL;
	}
	if (m_glDrawer != NULL)
	{
		m_glDrawer->Release();
		delete m_glDrawer;
		m_glDrawer = NULL;
	}
	if (m_renderer != NULL)
	{
		m_renderer->Release();
		delete m_renderer;
		m_renderer = NULL;
	}
	m_activeMaterial = NULL;
	m_globalRenderShader = NULL;
}
DGLCore * DGraphics::GetGLCore()
{
	return m_GL;
}

IImGUICore * DGraphics::GetGUICore()
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

void DGraphics::BeginScene(bool clearDepth, bool clearStencil, bool clearColor, DColor & color, DRenderTexture * renderTexture)
{
	if (renderTexture != NULL)
	{
		
		SetRenderTarget(renderTexture);
		Clear(clearDepth, clearStencil, clearColor, color, renderTexture);
	}
	else
	{
		SetRenderTarget();
		Clear(clearDepth, clearStencil, clearColor, color);
		
	}
	
	DSystem::GetGraphicsMgr()->ClearRenderQueue();
}

void DGraphics::EndScene(DRenderTexture * renderTexture)
{
	DSystem::GetGraphicsMgr()->ExecuteRenderQueue();

	if (renderTexture != NULL)
	{
		EndSetRenderTarget(renderTexture);
	}
	else
	{
		EndSetRenderTarget();
	}
}

void DGraphics::Clear(bool clearDepth, bool clearStencil, bool clearColor, DColor & color, DRenderTexture * renderTexture)
{
	if (renderTexture != NULL)
	{
		IRenderTextureViewWrapper* wrapper = renderTexture->GetTextureWrapper();
		if (wrapper != NULL)
			DSystem::GetGraphicsMgr()->GetGLCore()->Clear(clearDepth, clearStencil, clearColor, color, wrapper);
	}
	else
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->Clear(clearDepth, clearStencil, clearColor, color);
	}
}

void DGraphics::SetRenderTarget(DRenderTexture * res)
{
	if (res != NULL)
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->SetRenderTarget(res->GetTextureWrapper());
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
		DSystem::GetGraphicsMgr()->GetGLCore()->EndSetRenderTarget(res->GetTextureWrapper());
	}
	else
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->EndSetRenderTarget();
	}
}

void DGraphics::DrawGeometry(DGeometry * geometry, const DMatrix4x4 & matrix, DMaterial * material)
{
	if (geometry == NULL || material == NULL)
		return;
	DMatrix4x4 view, proj;
	/*camera->GetViewMatrix(view);
	camera->GetProjection(proj);*/

	DGraphics::GetModelView(view);
	DGraphics::GetProjection(proj);

	DMatrix4x4 world = matrix;

	material->SetMatrix(D_SC_MATRIX_M, world);
	material->SetMatrix(D_SC_MATRIX_V, view);
	material->SetMatrix(D_SC_MATRIX_P, proj);

	int passcount = material->GetPassCount();
	int i;
	for (i = 0; i < passcount; i++)
	{
		if (material->SetPass(i))
		{
			geometry->Draw(material->GetVertexUsage(i));

			DSystem::GetGraphicsMgr()->m_drawCall += 1;
		}
	}

}

void DGraphics::PushRenderQueue(DDisplayObject * displayObject, DRenderQueue renderQueue)
{
	if (DSystem::GetGraphicsMgr()->m_renderer != NULL)
	{
		DSystem::GetGraphicsMgr()->m_renderer->PushDisplayObject(displayObject, renderQueue);
	}
}

void DGraphics::Blit(DTexture * src, DMaterial * material, int pass)
{
	DGraphics::BeginScene(true, true, true, DCOLOR_WHITE);
	BlitInternal(src, material, pass);
	DGraphics::EndScene();
}

void DGraphics::Blit(DTexture * src, DRenderTexture * dst)
{
	DMaterial* blitCopy = DRes::LoadInternal<DMaterial>(D_RES_MAT_BLIT_COPY);
	Blit(src, dst, blitCopy);
}

void DGraphics::Blit(DTexture * src, DRenderTexture * dst, DMaterial * material, int pass)
{
	float rtw, rth;
	rtw = dst->GetWidth();
	rth = dst->GetHeight();
	DGraphics::SetViewPort(0, 0, rtw, rth);

	DGraphics::BeginScene(true, true, false, DCOLOR_BLACK, dst);

	BlitInternal(src, material, pass);

	DGraphics::EndScene(dst);
	float screenWidth, screenHeight;
	DSystem::GetGraphicsMgr()->GetGLCore()->GetResolution(screenWidth, screenHeight);
	DGraphics::SetViewPort(0, 0, screenWidth, screenHeight);
}

void DGraphics::DrawTexture(DTexture * src)
{
	DMaterial* blitCopy = DRes::LoadInternal<DMaterial>(D_RES_MAT_BLIT_COPY);
	Blit(src, blitCopy);
}

void DGraphics::DrawSkyBox(DMaterial * material, const DCamera * camera)
{
	if (material == NULL || camera == NULL)
		return;
	if (DSystem::GetGraphicsMgr()->m_skyGeometry == NULL)
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

	DrawGeometry(DSystem::GetGraphicsMgr()->m_skyGeometry, world, material);
}

void DGraphics::SetCullMode(DCullMode cullMode)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetCullMode(cullMode);
	}
}

void DGraphics::SetFillMode(DFillMode fillMode)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetFillMode(fillMode);
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

void DGraphics::SetBlendFactor(DRSBlendFactor src, DRSBlendFactor dst)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetBlendFactor(src, dst);
	}
}

void DGraphics::SetStencilOp(DRSStencilOp stencilFail, DRSStencilOp zFail, DRSStencilOp stencilPass)
{
	IRenderStateMgr* mgr = DSystem::GetGraphicsMgr()->GetGLCore()->GetRenderStateMgr();
	if (mgr != NULL)
	{
		mgr->SetStencilOp(stencilFail, zFail, stencilPass);
	}
}

void DGraphics::SetViewPort(float x, float y, float width, float height)
{
	DGLCore* gl = DSystem::GetGraphicsMgr()->GetGLCore();
	if (gl != NULL)
	{
		gl->SetViewPort(x, y, width, height);
	}
}

void DGraphics::ResetViewPort()
{
	DGLCore* gl = DSystem::GetGraphicsMgr()->GetGLCore();
	float w, h;
	gl->GetResolution(w, h);

	gl->SetViewPort(0, 0, w, h);
}

void DGraphics::SetGlobalRenderShader(DShader * shader)
{
	DGraphics* mgr = DSystem::GetGraphicsMgr();
	if (mgr != NULL)
	{
		mgr->m_globalRenderShader = shader;
	}
}

DShader * DGraphics::GetGlobalRenderShader()
{
	DGraphics* mgr = DSystem::GetGraphicsMgr();
	if (mgr != NULL)
	{
		return mgr->m_globalRenderShader;
	}
	return NULL;
}

void DGraphics::ClearGlobalRenderShader()
{
	DGraphics* mgr = DSystem::GetGraphicsMgr();
	if (mgr != NULL)
	{
		mgr->m_globalRenderShader = NULL;
	}
}

void DGraphics::GlBegin(DGeometryTopology mode)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlBegin(mode);
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

void DGraphics::GlVertex(DVector3 & vector)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlVertex(&vector);
	}
}

void DGraphics::GlVertex3(float x, float y, float z)
{
	DGLDrawer* drawer = DSystem::GetGraphicsMgr()->m_glDrawer;
	if (drawer != NULL)
	{
		drawer->GlVertex3(x, y, z);
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

//void DGraphics::PostGL()
//{
//	if (DSystem::GetGraphicsMgr()->m_glDrawer != NULL)
//	{
//		DSystem::GetGraphicsMgr()->m_glDrawer->PostGL();
//	}
//}

unsigned int DGraphics::GetDrawCall()
{
	return DSystem::GetGraphicsMgr()->m_drawCall;
}

void DGraphics::SetActiveMaterial(DMaterial * material)
{
	DSystem::GetGraphicsMgr()->m_activeMaterial = material;
}

void DGraphics::ClearActiveMaterial(DMaterial * material)
{
	DMaterial* currentActive = DSystem::GetGraphicsMgr()->m_activeMaterial;
	if (currentActive == material)
	{
		DSystem::GetGraphicsMgr()->m_activeMaterial = NULL;
	}
}

void DGraphics::ApplyActiveMaterial()
{
	DMaterial* currentActive = DSystem::GetGraphicsMgr()->m_activeMaterial;
	if (currentActive != NULL)
	{
		currentActive->ApplyPass();
	}
}

bool DGraphics::IsFrustrumZeroToOne()
{
	return DSystem::GetGraphicsMgr()->GetGLCore()->IsFrustrumZeroToOne();
}

void DGraphics::BlitInternal(DTexture* texture, DMaterial* material, int pass)
{
	if (DSystem::GetGraphicsMgr()->m_screenPlane == NULL)
	{
		DSystem::GetGraphicsMgr()->InitScreenPlane();
	}
	DMatrix4x4 world, view, proj;
	DMatrix4x4::Identity(&world);
	view = DMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 1.0f);
	float screenWidth, screenHeight;
	DSystem::GetGraphicsMgr()->GetGLCore()->GetResolution(screenWidth, screenHeight);
	DMatrix4x4::Ortho(&proj, screenWidth, screenHeight, -100.0f, 100.0f);

	material->SetMatrix(D_SC_MATRIX_M, world);
	material->SetMatrix(D_SC_MATRIX_V, view);
	material->SetMatrix(D_SC_MATRIX_P, proj);
	material->SetTexture(D_SC_TEXTURE_SCREEN, texture);

	
	if (pass < 0)
	{
		int passcount = material->GetPassCount();
		int i;
		for (i = 0; i < passcount; i++)
		{
			if (material->SetPass(i))
			{
				DSystem::GetGraphicsMgr()->m_screenPlane->Draw(material->GetVertexUsage(i));

				DSystem::GetGraphicsMgr()->m_drawCall += 1;
			}
		}
	}
	else
	{
		if (material->SetPass(pass))
		{
			DSystem::GetGraphicsMgr()->m_screenPlane->Draw(material->GetVertexUsage(pass));

			DSystem::GetGraphicsMgr()->m_drawCall += 1;
		}
	}
}

void DGraphics::InitScreenPlane()
{
	int vertexCount = 8;
	int indexCount = 30;

	float *vertices,*uvs;
	unsigned int *indexBuffer;

	vertices = new float[vertexCount * 3];
	uvs = new float[vertexCount * 2];
	indexBuffer = new unsigned int[indexCount];

	float screenWidth, screenHeight;
	m_GL->GetResolution(screenWidth, screenHeight);

	float bottom = -0.5f*screenHeight, bottomEx = -0.7f*screenHeight;
	float top = 0.5f*screenHeight, topEx = 0.7f*screenHeight;
	float left = -0.5f*screenWidth, leftEx = -0.7f*screenWidth;
	float right = 0.5f*screenWidth, rightEx = 0.7f*screenWidth;

	/*float bottom = 0.05f*screenHeight, bottomEx = 0.0f*screenHeight;
	float top = 0.45f*screenHeight, topEx = 0.5f*screenHeight;
	float left = 0.05f*screenWidth, leftEx = 0.0f*screenWidth;
	float right = 0.45f*screenWidth, rightEx = 0.5f*screenWidth;*/

	vertices[0] = left; vertices[1] = bottom; vertices[2] = 0.0f;
	vertices[3] = left; vertices[4] = top; vertices[5] = 0.0f;
	vertices[6] = right; vertices[7] = top; vertices[8] = 0.0f;
	vertices[9] = right; vertices[10] = bottom; vertices[11] = 0.0f;

	vertices[12] = leftEx; vertices[13] = bottomEx; vertices[14] = 0.0f;
	vertices[15] = leftEx; vertices[16] = topEx; vertices[17] = 0.0f;
	vertices[18] = rightEx; vertices[19] = topEx; vertices[20] = 0.0f;
	vertices[21] = rightEx; vertices[22] = bottomEx; vertices[23] = 0.0f;

	if (m_GL->IsUVStartsAtTop())
	{
		uvs[0] = 0.0f; uvs[1] = 1.0f;
		uvs[2] = 0.0f; uvs[3] = 0.0f;
		uvs[4] = 1.0f; uvs[5] = 0.0f;
		uvs[6] = 1.0f; uvs[7] = 1.0f;

		uvs[8] = 0.0f; uvs[9] = 1.0f;
		uvs[10] = 0.0f; uvs[11] = 0.0f;
		uvs[12] = 1.0f; uvs[13] = 0.0f;
		uvs[14] = 1.0f; uvs[15] = 1.0f;
	}
	else
	{
		uvs[0] = 0.0f; uvs[1] = 0.0f;
		uvs[2] = 0.0f; uvs[3] = 1.0f;
		uvs[4] = 1.0f; uvs[5] = 1.0f;
		uvs[6] = 1.0f; uvs[7] = 0.0f;

		uvs[8] = 0.0f; uvs[9] = 0.0f;
		uvs[10] = 0.0f; uvs[11] = 1.0f;
		uvs[12] = 1.0f; uvs[13] = 1.0f;
		uvs[14] = 1.0f; uvs[15] = 0.0f;
	}

	indexBuffer[0] = 0; indexBuffer[1] = 1; indexBuffer[2] = 2;
	indexBuffer[3] = 0; indexBuffer[4] = 2; indexBuffer[5] = 3;

	indexBuffer[6] = 0; indexBuffer[7] = 3; indexBuffer[8] = 7;
	indexBuffer[9] = 0; indexBuffer[10] = 7; indexBuffer[11] = 4;

	indexBuffer[12] = 0; indexBuffer[13] = 5; indexBuffer[14] = 1;
	indexBuffer[15] = 0; indexBuffer[16] = 4; indexBuffer[17] = 5;

	indexBuffer[18] = 1; indexBuffer[19] = 5; indexBuffer[20] = 6;
	indexBuffer[21] = 1; indexBuffer[22] = 6; indexBuffer[23] = 2;

	indexBuffer[24] = 2; indexBuffer[25] = 6; indexBuffer[26] = 7;
	indexBuffer[27] = 2; indexBuffer[28] = 7; indexBuffer[29] = 3;

	m_screenPlane = new DGeometry();
	m_screenPlane->SetVertices(vertices, 8);
	m_screenPlane->SetUVs(0, uvs, 8);
	m_screenPlane->SetIndices(indexBuffer, 30);

}

void DGraphics::InitSkyBox()
{
	m_skyGeometry = new DGeometry();
	float *vertices = new float[108];
	unsigned int* indices = new unsigned int[36];

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

	m_skyGeometry->SetVertices(vertices, 36);
	m_skyGeometry->SetIndices(indices, 36);
}

void DGraphics::ClearRenderQueue()
{
	if (m_renderer != NULL)
		m_renderer->Clear();
}

void DGraphics::ExecuteRenderQueue()
{
	if (m_renderer != NULL)
		m_renderer->Render();
}

