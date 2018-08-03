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
	glEnable(GL_CULL_FACE);

	ChangeCullMode(DCullMode_Back);
	ChangeFillMode(DFillMode_Solid);
	ChangeZTest(DRSCompareFunc_LEqual);
	ChangeZWrite(true);
	ChangeBlendEnable(false);
	ChangeBlendFactor(DRSBlendFactor_One, DRSBlendFactor_Zero);
	ChangeBlendOp(DRSBlendOp_Add);
	ChangeStencilEnable(false);
	ChangeStencilId(0);
	ChangeStencilWriteMask(0xff);
	ChangeStencilReadMask(0xff);
	ChangeStencilCompFunc(DRSCompareFunc_Always);
	//ChangeStencilPassOp(DRSStencilOp_Keep);
	//ChangeStencilFailOp(DRSStencilOp_Keep);
	//ChangeStencilZFailOp(DRSStencilOp_Keep);
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
	ChangeBlendFactor(src, m_blendDstFactor);
}

void DRenderStateMgrOpGL::SetBlendDstFactor(DRSBlendFactor dst)
{
	if (m_blendDstFactor == dst)
		return;
	ChangeBlendFactor(m_blendSrcFactor, dst);
}

void DRenderStateMgrOpGL::SetBlendFactor(DRSBlendFactor src, DRSBlendFactor dst)
{
	if (m_blendSrcFactor == src && m_blendDstFactor == dst)
		return;
	ChangeBlendFactor(src, dst);
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
	//ChangeStencilPassOp(stencilPass);
}

void DRenderStateMgrOpGL::SetStencilFailOp(DRSStencilOp stencilFail)
{
	if (m_stencilFail == stencilFail)
		return;
	//ChangeStencilFailOp(stencilFail);
}

void DRenderStateMgrOpGL::SetStencilZFailOp(DRSStencilOp stencilZFail)
{
	if (m_stencilZFail == stencilZFail)
		return;
	//ChangeStencilZFailOp(stencilZFail);
}

void DRenderStateMgrOpGL::SetStencilOp(DRSStencilOp stencilFail, DRSStencilOp zFail, DRSStencilOp stencilPass)
{
}

void DRenderStateMgrOpGL::ChangeCullMode(DCullMode cullMode)
{
	if (cullMode == DCullMode_Back)
		glCullFace(GL_FRONT);
	else if (cullMode == DCullMode_Front)
		glCullFace(GL_BACK);
	else if (cullMode == DCullMode_Off)
		glCullFace(GL_FRONT_AND_BACK);
	m_currentCullMode = cullMode;
}

void DRenderStateMgrOpGL::ChangeFillMode(DFillMode fillMode)
{
	if (fillMode == DFillMode_Solid)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (fillMode == DFillMode_WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_currentFillMode = fillMode;
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

void DRenderStateMgrOpGL::ChangeBlendEnable(bool enable)
{
	if (enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	m_enableBlend = enable;
}

void DRenderStateMgrOpGL::ChangeBlendOp(DRSBlendOp op)
{
	glBlendEquation(GetBlendOp(op));
	m_blendOp = op;
}

void DRenderStateMgrOpGL::ChangeBlendFactor(DRSBlendFactor src, DRSBlendFactor dst)
{
	glBlendFunc(GetBlendFactor(src), GetBlendFactor(dst));
	m_blendSrcFactor = src;
	m_blendDstFactor = dst;
}

void DRenderStateMgrOpGL::ChangeStencilEnable(bool enable)
{
	if (enable)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);
	m_enableStencil = enable;
}

void DRenderStateMgrOpGL::ChangeStencilId(unsigned short)
{
}

void DRenderStateMgrOpGL::ChangeStencilCompFunc(DRSCompareFunc)
{
}

void DRenderStateMgrOpGL::ChangeStencilOp(DRSStencilOp stencilFail, DRSStencilOp zFail, DRSStencilOp stencilPass)
{
	glStencilOp(GetStencilOp(stencilFail), GetStencilOp(zFail), GetStencilOp(stencilPass));
	m_stencilFail = stencilFail;
	m_stencilPass = stencilPass;
	m_stencilZFail = zFail;
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

GLenum DRenderStateMgrOpGL::GetBlendOp(DRSBlendOp op)
{
	switch (op)
	{
	case DRSBlendOp_Add:
		return GL_FUNC_ADD;
	case DRSBlendOp_Sub:
		return GL_FUNC_SUBTRACT;
	case DRSBlendOp_RevSub:
		return GL_FUNC_REVERSE_SUBTRACT;
	case DRSBlendOp_Min:
		throw "not supported";
	case DRSBlendOp_Max:
		throw "not supported";
	default:
		return GL_FUNC_ADD;
	}
}

GLenum DRenderStateMgrOpGL::GetBlendFactor(DRSBlendFactor factor)
{
	switch (factor)
	{
	case DRSBlendFactor_One:
		return GL_ONE;
	case DRSBlendFactor_Zero:
		return GL_ZERO;
	case DRSBlendFactor_SrcColor:
		return GL_SRC_COLOR;
	case DRSBlendFactor_SrcAlpha:
		return GL_SRC_ALPHA;
	case DRSBlendFactor_DstColor:
		return GL_DST_COLOR;
	case DRSBlendFactor_DstAlpha:
		return GL_DST_ALPHA;
	case DRSBlendFactor_OneMinusSrcColor:
		return GL_ONE_MINUS_SRC_COLOR;
	case DRSBlendFactor_OneMinusSrcAlpha:
		return GL_ONE_MINUS_SRC_ALPHA;
	case DRSBlendFactor_OneMinusDstColor:
		return GL_ONE_MINUS_DST_COLOR;
	case DRSBlendFactor_OneMinusDstAlpha:
		return GL_ONE_MINUS_DST_ALPHA;
	default:
		return GL_ZERO;
	}
}

GLenum DRenderStateMgrOpGL::GetStencilOp(DRSStencilOp op)
{
	switch (op)
	{
	case DRSStencilOp_Keep:
		return GL_KEEP;
	case DRSStencilOp_Zero:
		return GL_ZERO;
	case DRSStencilOp_Replace:
		return GL_REPLACE;
	case DRSStencilOp_IncrementSaturate:
		return GL_INCR;
	case DRSStencilOp_DecrementSaturate:
		return GL_DECR;
	case DRSStencilOp_Invert:
		return GL_INVERT;
	case DRSStencilOp_IncrementWrap:
		return GL_INCR_WRAP;
	case DRSStencilOp_DecrementWrap:
		return GL_DECR_WRAP;
	default:
		return GL_KEEP;
	}
}

#endif