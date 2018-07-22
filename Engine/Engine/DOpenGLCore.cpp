#ifdef _DGAPI_OPENGL
#include "DOpenGLCore.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

DOpenGLCore::DOpenGLCore()
{
}


DOpenGLCore::~DOpenGLCore()
{
}

bool DOpenGLCore::Init(int width, int height, bool fullscreen, HWND)
{

	return true;
}

void DOpenGLCore::Destroy()
{
}

void DOpenGLCore::Present()
{
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