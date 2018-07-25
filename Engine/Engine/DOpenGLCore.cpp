#ifdef _DGAPI_OPENGL
#include "DOpenGLCore.h"

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

	m_window = window;

	return true;
}

void DOpenGLCore::Destroy()
{
	glfwTerminate();
}

void DOpenGLCore::Present()
{
	glfwSwapBuffers(m_window);
}

void DOpenGLCore::Clear(bool clearDepth, bool clearStencil, bool clearColor, DColor & color, IRenderTextureViewRes *)
{
	GLbitfield clearFlag = 0;
	if (clearDepth)
		clearFlag |= GL_DEPTH_BUFFER_BIT;
	if (clearStencil)
		clearFlag |= GL_STENCIL_BUFFER_BIT;
	if (clearColor)
		clearFlag |= GL_COLOR_BUFFER_BIT;
	glClear(clearFlag);
	glClearColor(color.r, color.g, color.b, color.a);
}

void DOpenGLCore::SetRenderTarget(IRenderTextureViewRes *)
{
}

void DOpenGLCore::SetViewPort(float x, float y, float width, float height)
{
	glViewport(x, y, width, height);
}

void DOpenGLCore::EndSetRenderTarget(IRenderTextureViewRes *)
{
}

DGeometryRes * DOpenGLCore::CreateGeometryRes(int vertexUsage, bool dynamic)
{
	return nullptr;
}

ITextureRes * DOpenGLCore::CreateTextureRes(WCHAR *)
{
	return nullptr;
}

IRenderTextureViewRes * DOpenGLCore::CreateRenderTextureRes(float width, float height)
{
	return nullptr;
}

DShaderPass * DOpenGLCore::CreateShaderPass()
{
	return nullptr;
}

void DOpenGLCore::ApplySamplerState(UINT, DWrapMode)
{
}

IRenderStateMgr * DOpenGLCore::GetRenderStateMgr()
{
	return nullptr;
}
#endif