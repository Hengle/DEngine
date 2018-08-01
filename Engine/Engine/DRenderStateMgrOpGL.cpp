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

	ChangeCullMode(DCullMode_Back);
	ChangeFillMode(DFillMode_Solid);
	ChangeZTest(DRSCompareFunc_LEqual);
	ChangeZWrite(true);
	ChangeBlendEnable(false);
	ChangeBlendSrcFactor(DRSBlendFactor_One);
	ChangeBlendDstFactor(DRSBlendFactor_Zero);
	ChangeBlendOp(DRSBlendOp_Add);
	ChangeStencilEnable(false);
	ChangeStencilId(0);
	ChangeStencilWriteMask(0xff);
	ChangeStencilReadMask(0xff);
	ChangeStencilCompFunc(DRSCompareFunc_Always);
	ChangeStencilPassOp(DRSStencilOp_Keep);
	ChangeStencilFailOp(DRSStencilOp_Keep);
	ChangeStencilZFailOp(DRSStencilOp_Keep);
}

void DRenderStateMgrOpGL::Release()
{
}

void DRenderStateMgrOpGL::SetCullMode(DCullMode cullMode)
{
	if (m_currentCullMode == cullMode)
		return;
	ChangeCullMode(cullMode);
}

void DRenderStateMgrOpGL::SetFillMode(DFillMode fillMode)
{
	if (m_currentFillMode == fillMode)
		return;
	ChangeFillMode(fillMode);
}

void DRenderStateMgrOpGL::SetZWriteEnable(bool zwrite)
{
	if (m_zwrite == zwrite)
		return;
	ChangeZWrite(zwrite);
}

void DRenderStateMgrOpGL::SetZTestFunc(DRSCompareFunc ztest)
{
	if (m_ztest == ztest)
		return;
	ChangeZTest(ztest);
}

void DRenderStateMgrOpGL::SetBlendOp(DRSBlendOp op)
{
	if (m_blendOp == op)
		return;
	ChangeBlendOp(op);
}

void DRenderStateMgrOpGL::SetBlendEnable(bool enable)
{
	if (m_enableBlend == enable)
		return;
	ChangeBlendEnable(enable);
}

void DRenderStateMgrOpGL::SetBlendSrcFactor(DRSBlendFactor src)
{
	if (m_blendSrcFactor == src)
		return;
	ChangeBlendSrcFactor(src);
}

void DRenderStateMgrOpGL::SetBlendDstFactor(DRSBlendFactor dst)
{
	if (m_blendDstFactor == dst)
		return;
	ChangeBlendDstFactor(dst);
}

void DRenderStateMgrOpGL::SetStencilRefId(UINT stencilId)
{
	if (m_stencilId == stencilId)
		return;
	ChangeStencilId(stencilId);
}

void DRenderStateMgrOpGL::SetStencilEnable(bool enableStencil)
{
	if (m_enableStencil == enableStencil)
		return;
	ChangeStencilEnable(enableStencil);
}

void DRenderStateMgrOpGL::SetStencilReadMask(unsigned short readMask)
{
	if (m_stencilReadMask == readMask)
		return;
	ChangeStencilReadMask(readMask);
}

void DRenderStateMgrOpGL::SetStencilWriteMask(unsigned short writeMask)
{
	if (m_stencilWriteMask == writeMask)
		return;
	ChangeStencilWriteMask(writeMask);
}

void DRenderStateMgrOpGL::SetStencilComparisonFunc(DRSCompareFunc stencilComp)
{
	if (m_stencilComp == stencilComp)
		return;
	ChangeStencilCompFunc(stencilComp);
}

void DRenderStateMgrOpGL::SetStencilPassOp(DRSStencilOp stencilPass)
{
	if (m_stencilPass == stencilPass)
		return;
	ChangeStencilPassOp(stencilPass);
}

void DRenderStateMgrOpGL::SetStencilFailOp(DRSStencilOp stencilFail)
{
	if (m_stencilFail == stencilFail)
		return;
	ChangeStencilFailOp(stencilFail);
}

void DRenderStateMgrOpGL::SetStencilZFailOp(DRSStencilOp stencilZFail)
{
	if (m_stencilZFail == stencilZFail)
		return;
	ChangeStencilZFailOp(stencilZFail);
}

void DRenderStateMgrOpGL::ChangeCullMode(DCullMode)
{
}

void DRenderStateMgrOpGL::ChangeFillMode(DFillMode)
{
}

void DRenderStateMgrOpGL::ChangeZWrite(bool zwrite)
{
	if (zwrite)
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);
	m_zwrite = zwrite;
}

void DRenderStateMgrOpGL::ChangeZTest(DRSCompareFunc ztest)
{
	glDepthFunc(GetComparisonFunc(ztest));
	m_ztest = ztest;
}

void DRenderStateMgrOpGL::ChangeBlendEnable(bool)
{
}

void DRenderStateMgrOpGL::ChangeBlendOp(DRSBlendOp)
{
}

void DRenderStateMgrOpGL::ChangeBlendSrcFactor(DRSBlendFactor)
{
}

void DRenderStateMgrOpGL::ChangeBlendDstFactor(DRSBlendFactor)
{
}

void DRenderStateMgrOpGL::ChangeStencilEnable(bool)
{
}

void DRenderStateMgrOpGL::ChangeStencilId(unsigned short)
{
}

void DRenderStateMgrOpGL::ChangeStencilCompFunc(DRSCompareFunc)
{
}

void DRenderStateMgrOpGL::ChangeStencilPassOp(DRSStencilOp)
{
}

void DRenderStateMgrOpGL::ChangeStencilFailOp(DRSStencilOp)
{
}

void DRenderStateMgrOpGL::ChangeStencilZFailOp(DRSStencilOp)
{
}

void DRenderStateMgrOpGL::ChangeStencilReadMask(unsigned short)
{
}

void DRenderStateMgrOpGL::ChangeStencilWriteMask(unsigned short)
{
}

GLenum DRenderStateMgrOpGL::GetComparisonFunc(DRSCompareFunc func)
{
	switch (func)
	{
	case DRSCompareFunc_Always:
		return GL_ALWAYS;
	case DRSCompareFunc_Equal:
		return GL_EQUAL;
	case DRSCompareFunc_GEqual:
		return GL_GEQUAL;
	case DRSCompareFunc_Greater:
		return GL_GREATER;
	case DRSCompareFunc_LEqual:
		return GL_LEQUAL;
	case DRSCompareFunc_Less:
		return GL_LESS;
	case DRSCompareFunc_Never:
		return GL_NEVER;
	case DRSCompareFunc_NotEqual:
		return GL_NOTEQUAL;
	default:
		return GL_LEQUAL;
	}
}

#endif