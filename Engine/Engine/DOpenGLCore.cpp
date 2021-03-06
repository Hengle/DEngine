﻿#ifdef _DGAPI_OPENGL
#include "DOpenGLCore.h"
#include "DShaderPassOpGL.h"
#include "DGeometryWrapperOpGL.h"
#include "DTextureWrapperOpGL.h"
#include "DRenderStateMgrOpGL.h"

DOpenGLCore::DOpenGLCore()
{
}


DOpenGLCore::~DOpenGLCore()
{
}

bool DOpenGLCore::Init(int width, int height, bool fullscreen, GLFWwindow* window)
{
	DGLCore::Init(width, height, fullscreen);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		MessageBox(NULL, L"创建GLAD失败", L"初始化失败", MB_ERR_INVALID_CHARS);
		return false;

	}

	InitRenderStateMgr();

	//glFrontFace(GL_CW);

	m_window = window;

	return true;
}

void DOpenGLCore::Destroy()
{
	glfwTerminate();
	if (m_renderStateMgr != NULL)
	{
		m_renderStateMgr->Release();
		delete m_renderStateMgr;
		m_renderStateMgr = NULL;
	}
}

void DOpenGLCore::Present()
{
	glfwSwapBuffers(m_window);
}

void DOpenGLCore::Clear(bool clearDepth, bool clearStencil, bool clearColor, DColor & color, IRenderTextureViewWrapper *)
{
	GLbitfield clearFlag = 0;
	if (clearDepth)
		clearFlag |= GL_DEPTH_BUFFER_BIT;
	if (clearStencil)
		clearFlag |= GL_STENCIL_BUFFER_BIT;
	if (clearColor)
		clearFlag |= GL_COLOR_BUFFER_BIT;
	glClear(clearFlag);
	if(clearColor)
		glClearColor(color.r, color.g, color.b, color.a);
}

void DOpenGLCore::SetRenderTarget(IRenderTextureViewWrapper * view)
{
	if (view != NULL)
	{
		((DRenderTextureViewWrapperOpGL*)view)->BindFBO();
	}
	else
	{
		((DRenderTextureViewWrapperOpGL*)view)->UnBindFBO();
	}
}

void DOpenGLCore::SetViewPort(float x, float y, float width, float height)
{
	glViewport(x, y, width, height);
}

void DOpenGLCore::EndSetRenderTarget(IRenderTextureViewWrapper * view)
{
	if (view != NULL)
	{
		((DRenderTextureViewWrapperOpGL*)view)->UnBindFBO();
	}
}

DGeometryWrapper * DOpenGLCore::CreateGeometryWrapper(int vertexUsage, bool dynamic)
{
	return new DGeometryWrapperOpGL(vertexUsage, dynamic);
}

ITextureWrapper * DOpenGLCore::CreateTextureWrapper(WCHAR * filename)
{
	return new DTextureWrapperOpGL(filename, DWrapMode_Clamp);;
}

ITextureWrapper * DOpenGLCore::CreateCubeMapWrapper(ITextureWrapper * right, ITextureWrapper * left, ITextureWrapper * top, ITextureWrapper * bottom, ITextureWrapper * front, ITextureWrapper * back)
{
	return new DTextureWrapperOpGL((DTextureWrapperOpGL*)right, (DTextureWrapperOpGL*)left, (DTextureWrapperOpGL*)top, (DTextureWrapperOpGL*)bottom, (DTextureWrapperOpGL*)front, (DTextureWrapperOpGL*)back);
}

IRenderTextureViewWrapper * DOpenGLCore::CreateRenderTextureWrapper(float width, float height)
{
	return new DRenderTextureViewWrapperOpGL(width, height, DWrapMode_Clamp);
}

DShaderPass * DOpenGLCore::CreateShaderPass()
{
	return new DShaderPassOpGL();
}

void DOpenGLCore::ApplySamplerState(UINT, DWrapMode)
{
}

IRenderStateMgr * DOpenGLCore::GetRenderStateMgr()
{
	return m_renderStateMgr;
}
void DOpenGLCore::InitRenderStateMgr()
{
	m_renderStateMgr = new DRenderStateMgrOpGL();
	m_renderStateMgr->Init();
}
#endif