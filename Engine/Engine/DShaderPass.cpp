#include "DShaderPass.h"
#include "DGraphics.h"
#include <string>

DShaderProgram::DShaderProgram()
{
	m_isInitialized = false;
	m_cbufferCount = 0;
	m_propertyCount = 0;
	m_vertexUsage = 0;
	m_resCount = 0;
}

unsigned int DShaderProgram::GetPropertyCount() const
{
	return m_propertyCount;
}

//void DShaderProgram::Init(const char * content, char * funcName)
//{
//	m_isInitialized = OnInit(content, funcName);
//}

unsigned int DShaderProgram::GetResCount() const
{
	return m_resCount;
}

void DShaderProgram::ApplyParams(std::map<std::string, float*>& params, std::map<std::string, float*>&gparams)
{
	if (m_isInitialized)
		OnApplyParams(params, gparams);
}

void DShaderProgram::Draw()
{
	if (m_isInitialized)
		OnDraw();
}

bool DShaderProgram::IsInitialized()
{
	return m_isInitialized;
}

int DShaderProgram::GetVertexUsage()
{
	return m_vertexUsage;
}

DShaderPass::DShaderPass()
{
	m_zwrite = true;
	m_ztest = DRSCompareFunc_LEqual;
	m_cullmode = DCullMode_Back;
	m_fillmode = DFillMode_Solid;
	m_enableBlend = false;
	m_blendOp = DRSBlendOp_Add;
	m_blendSrc = DRSBlendFactor_SrcAlpha;
	m_blendDst = DRSBlendFactor_OneMinusSrcAlpha;
	m_enableStencil = false;
	m_stencilId = 0;
	m_stencilReadMask = 255;
	m_stencilWriteMask = 255;
	m_stencilComp = DRSCompareFunc_Always;
	m_stencilPass = DRSStencilOp_Keep;
	m_stencilFail = DRSStencilOp_Keep;
	m_stencilZFail = DRSStencilOp_Keep;
	m_passEnable = true;
}

void DShaderPass::Release()
{
}

void DShaderPass::SetZWrite(char * state)
{
	if (strcmp(state, "on") == 0)
		m_zwrite = true;
	else if (strcmp(state, "off") == 0)
		m_zwrite = false;
}

void DShaderPass::SetZTest(char * state)
{
	m_ztest = GetCompFunc(state);
}

void DShaderPass::SetCullMode(char * state)
{
	if (strcmp(state, "off") == 0)
		m_cullmode = DCullMode_Off;
	else if (strcmp(state, "back") == 0)
		m_cullmode = DCullMode_Back;
	else if (strcmp(state, "front") == 0)
		m_cullmode = DCullMode_Front;
}

void DShaderPass::SetFillMode(char * state)
{
	if (strcmp(state, "solid") == 0)
		m_fillmode = DFillMode_Solid;
	else if (strcmp(state, "wireframe") == 0)
		m_fillmode = DFillMode_WireFrame;
}

void DShaderPass::SetBlendOp(char * state)
{
	if (strcmp(state, "add") == 0)
		m_blendOp = DRSBlendOp_Add;
	else if (strcmp(state, "sub") == 0)
		m_blendOp = DRSBlendOp_Sub;
	else if (strcmp(state, "revsub") == 0)
		m_blendOp = DRSBlendOp_RevSub;
	else if (strcmp(state, "min") == 0)
		m_blendOp = DRSBlendOp_Min;
	else if (strcmp(state, "max") == 0)
		m_blendOp = DRSBlendOp_Max;
}

void DShaderPass::SetBlendSrcFactor(char * state)
{
	m_blendSrc = GetBlendFactor(state);
	m_enableBlend = true;
}

void DShaderPass::SetBlendDstFactor(char * state)
{
	m_blendDst = GetBlendFactor(state);
	m_enableBlend = true;
}

void DShaderPass::SetStencilEnable(bool enable)
{
	m_enableStencil = enable;
}

void DShaderPass::SetStencilId(unsigned short stencilId)
{
	m_stencilId = stencilId;
}

void DShaderPass::SetStencilReadMask(unsigned short readMask)
{
	m_stencilReadMask = readMask;
}

void DShaderPass::SetStencilWriteMask(unsigned short writeMask)
{
	m_stencilWriteMask = writeMask;
}

void DShaderPass::SetStencilComp(char * state)
{
	m_stencilComp = GetCompFunc(state);
}

void DShaderPass::SetStencilPass(char * state)
{
	m_stencilPass = GetStencilOp(state);
}

void DShaderPass::SetStencilFail(char * state)
{
	m_stencilFail = GetStencilOp(state);
}

void DShaderPass::SetStencilZFail(char * state)
{
	m_stencilZFail = GetStencilOp(state);
}

//void DShaderPass::SetVertexFuncName(char *vertexFuncName)
//{
//	int len = strlen(vertexFuncName) + 1;
//	m_vertexFuncName = new char[len];
//	strcpy_s(m_vertexFuncName, len, vertexFuncName);
//}
//
//void DShaderPass::SetPixelFuncName(char *pixelFuncName)
//{
//	int len = strlen(pixelFuncName) + 1;
//	m_pixelFuncName = new char[len];
//	strcpy_s(m_pixelFuncName, len, pixelFuncName);
//}

//void DShaderPass::CompileShader(const char * content)
//{
//	/*DShaderRes* res = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderRes();
//	if (res != NULL)
//	{
//	res->Init(content, m_vertexFuncName, m_pixelFuncName);
//	m_shaderRes = res;
//	}*/
//	DShaderProgram* vshader = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderProgram(DShaderProgram_Vertex);
//	DShaderProgram* pshader = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderProgram(DShaderProgram_Pixel);
//	if (vshader != NULL)
//	{
//		vshader->Init(content, m_vertexFuncName);
//		m_vertexShader = vshader;
//	}
//	if (pshader != NULL)
//	{
//		pshader->Init(content, m_pixelFuncName);
//		m_pixelShader = pshader;
//	}
//}
//
//DShaderProgram * DShaderPass::GetVertexShader()
//{
//	return m_vertexShader;
//}
//
//DShaderProgram * DShaderPass::GetPixelShader()
//{
//	return m_pixelShader;
//}

//DShaderRes * DShaderPass::GetShaderRes()
//{
//	return m_shaderRes;
//}

void DShaderPass::ApplyStates()
{
	DGraphics::SetCullMode(m_cullmode);
	DGraphics::SetFillMode(m_fillmode);
	DGraphics::SetZTestFunc(m_ztest);
	DGraphics::SetZWriteEnable(m_zwrite);
	DGraphics::SetBlendEnable(m_enableBlend);
	DGraphics::SetBlendOp(m_blendOp);
	//DGraphics::SetBlendDstFactor(m_blendDst);
	//DGraphics::SetBlendSrcFactor(m_blendSrc);
	DGraphics::SetBlendFactor(m_blendSrc, m_blendDst);
}

void DShaderPass::SetPassEnable(bool enable)
{
	m_passEnable = enable;
}

bool DShaderPass::IsPassEnable()
{
	return m_passEnable;
}

DRSBlendFactor DShaderPass::GetBlendFactor(char * state)
{
	if (strcmp(state, "one") == 0)
		return DRSBlendFactor_One;
	else if (strcmp(state, "zero") == 0)
		return DRSBlendFactor_Zero;
	else if (strcmp(state, "srccolor") == 0)
		return DRSBlendFactor_SrcColor;
	else if (strcmp(state, "srcalpha") == 0)
		return DRSBlendFactor_SrcAlpha;
	else if (strcmp(state, "dstcolor") == 0)
		return DRSBlendFactor_DstColor;
	else if (strcmp(state, "dstalpha") == 0)
		return DRSBlendFactor_DstAlpha;
	else if (strcmp(state, "oneminussrccolor") == 0)
		return DRSBlendFactor_OneMinusSrcColor;
	else if (strcmp(state, "oneminussrcalpha") == 0)
		return DRSBlendFactor_OneMinusSrcAlpha;
	else if (strcmp(state, "oneminusdstcolor") == 0)
		return DRSBlendFactor_OneMinusDstColor;
	else if (strcmp(state, "oneminusdstalpha") == 0)
		return DRSBlendFactor_OneMinusDstAlpha;
	return DRSBlendFactor_Zero;
}

DRSCompareFunc DShaderPass::GetCompFunc(char * state)
{
	if (strcmp(state, "lequal") == 0)
		return DRSCompareFunc_LEqual;
	else if (strcmp(state, "never") == 0)
		return DRSCompareFunc_Never;
	else if (strcmp(state, "less") == 0)
		return DRSCompareFunc_Less;
	else if (strcmp(state, "equal") == 0)
		return DRSCompareFunc_Equal;
	else if (strcmp(state, "greater") == 0)
		return DRSCompareFunc_Greater;
	else if (strcmp(state, "notequal") == 0)
		return DRSCompareFunc_NotEqual;
	else if (strcmp(state, "gequal") == 0)
		return DRSCompareFunc_GEqual;
	else if (strcmp(state, "always") == 0)
		return DRSCompareFunc_Always;
	else
		return DRSCompareFunc_Always;
}

DRSStencilOp DShaderPass::GetStencilOp(char * state)
{
	if (strcmp(state, "keep") == 0)
		return DRSStencilOp_Keep;
	else if (strcmp(state, "zero") == 0)
		return DRSStencilOp_Zero;
	else if (strcmp(state, "replace") == 0)
		return DRSStencilOp_Replace;
	else if (strcmp(state, "incrsat") == 0)
		return DRSStencilOp_IncrementSaturate;
	else if (strcmp(state, "decrsat") == 0)
		return DRSStencilOp_DecrementSaturate;
	else if (strcmp(state, "invert") == 0)
		return DRSStencilOp_Invert;
	else if (strcmp(state, "incrwrap") == 0)
		return DRSStencilOp_IncrementWrap;
	else if (strcmp(state, "decrwrap") == 0)
		return DRSStencilOp_DecrementWrap;
	else
		return DRSStencilOp_Keep;
}
