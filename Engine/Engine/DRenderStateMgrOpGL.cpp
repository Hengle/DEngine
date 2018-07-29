#ifdef _DGAPI_OPENGL

#include "DRenderStateMgrOpGL.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

DRenderStateMgrOpGL::DRenderStateMgrOpGL()
{
}


DRenderStateMgrOpGL::~DRenderStateMgrOpGL()
{
}

void DRenderStateMgrOpGL::Init()
{
	glEnable(GL_DEPTH_TEST);
}

void DRenderStateMgrOpGL::Release()
{
}

void DRenderStateMgrOpGL::SetCullMode(DCullMode)
{
}

void DRenderStateMgrOpGL::SetFillMode(DFillMode)
{
}

void DRenderStateMgrOpGL::SetZWriteEnable(bool zwrite)
{
	
}

void DRenderStateMgrOpGL::SetZTestFunc(DRSCompareFunc)
{
	glDepthFunc(GL_LESS);
}

void DRenderStateMgrOpGL::SetBlendOp(DRSBlendOp)
{
}

void DRenderStateMgrOpGL::SetBlendEnable(bool)
{
}

void DRenderStateMgrOpGL::SetBlendSrcFactor(DRSBlendFactor)
{
}

void DRenderStateMgrOpGL::SetBlendDstFactor(DRSBlendFactor)
{
}

void DRenderStateMgrOpGL::SetStencilRefId(UINT)
{
}

void DRenderStateMgrOpGL::SetStencilEnable(bool)
{
}

void DRenderStateMgrOpGL::SetStencilReadMask(unsigned short)
{
}

void DRenderStateMgrOpGL::SetStencilWriteMask(unsigned short)
{
}

void DRenderStateMgrOpGL::SetStencilComparisonFunc(DRSCompareFunc)
{
}

void DRenderStateMgrOpGL::SetStencilPassOp(DRSStencilOp)
{
}

void DRenderStateMgrOpGL::SetStencilFailOp(DRSStencilOp)
{
}

void DRenderStateMgrOpGL::SetStencilZFailOp(DRSStencilOp)
{
}

#endif