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

void DOpenGLCore::Clear(bool, bool, bool, DColor &, IRenderTextureViewRes *)
{
	
}

void DOpenGLCore::SetRenderTarget(IRenderTextureViewRes *)
{
}

void DOpenGLCore::SetViewPort(float, float, float, float)
{
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

DShaderProgram * DOpenGLCore::CreateShaderProgram(DShaderProgramType)
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