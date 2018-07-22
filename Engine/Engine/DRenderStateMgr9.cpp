#ifdef _DGAPI_D3D9
#include "DRenderStateMgr9.h"

DRenderStateMgr9::DRenderStateMgr9(IDirect3DDevice9* device)
{
	m_device = device;
	m_enableBlend = false;
}

DRenderStateMgr9::~DRenderStateMgr9()
{
}

void DRenderStateMgr9::Init()
{
	if (m_device == NULL)
		return;

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

void DRenderStateMgr9::Release()
{
	m_device = NULL;
}

void DRenderStateMgr9::SetCullMode(DCullMode cullMode)
{
	if (m_device == NULL)
		return;
	if (m_currentCullMode == cullMode)
		return;
	ChangeCullMode(cullMode);
}

void DRenderStateMgr9::SetFillMode(DFillMode fillMode)
{
	if (m_device == NULL)
		return;
	if (m_currentFillMode == fillMode)
		return;
	ChangeFillMode(fillMode);
}

void DRenderStateMgr9::SetZWriteEnable(bool zwrite)
{
	if (m_device == NULL)
		return;
	if (m_zwrite == zwrite)
		return;
	ChangeZWrite(zwrite);
}

void DRenderStateMgr9::SetZTestFunc(DRSCompareFunc ztest)
{
	if (m_device == NULL)
		return;
	if (m_ztest == ztest)
		return;
	ChangeZTest(ztest);
}

void DRenderStateMgr9::SetBlendOp(DRSBlendOp op)
{
	if (m_device == NULL)
		return;
	if (m_blendOp == op)
		return;
	ChangeBlendOp(op);
}

void DRenderStateMgr9::SetBlendEnable(bool enable)
{
	if (m_device == NULL)
		return;
	if (m_enableBlend == enable)
		return;
	ChangeBlendEnable(enable);
}

void DRenderStateMgr9::SetBlendSrcFactor(DRSBlendFactor src)
{
	if (m_device == NULL)
		return;
	if (m_blendSrcFactor == src)
		return;
	ChangeBlendSrcFactor(src);
}

void DRenderStateMgr9::SetBlendDstFactor(DRSBlendFactor dst)
{
	if (m_device == NULL)
		return;
	if (m_blendDstFactor == dst)
		return;
	ChangeBlendDstFactor(dst);
}

void DRenderStateMgr9::SetStencilRefId(UINT stencilId)
{
	if (m_device == NULL)
		return;
	if (m_stencilId == stencilId)
		return;
	ChangeStencilId(stencilId);
}

void DRenderStateMgr9::SetStencilEnable(bool enableStencil)
{
	if (m_device == NULL)
		return;
	if (m_enableStencil == enableStencil)
		return;
	ChangeStencilEnable(enableStencil);
}

void DRenderStateMgr9::SetStencilReadMask(unsigned short readMask)
{
	if (m_device == NULL)
		return;
	if (m_stencilReadMask == readMask)
		return;
	ChangeStencilReadMask(readMask);
}

void DRenderStateMgr9::SetStencilWriteMask(unsigned short writeMask)
{
	if (m_device == NULL)
		return;
	if (m_stencilWriteMask == writeMask)
		return;
	ChangeStencilWriteMask(writeMask);
}

void DRenderStateMgr9::SetStencilComparisonFunc(DRSCompareFunc stencilComp)
{
	if (m_device == NULL)
		return;
	if (m_stencilComp == stencilComp)
		return;
	ChangeStencilCompFunc(stencilComp);
}

void DRenderStateMgr9::SetStencilPassOp(DRSStencilOp stencilPass)
{
	if (m_device == NULL)
		return;
	if (m_stencilPass == stencilPass)
		return;
	ChangeStencilPassOp(stencilPass);
}

void DRenderStateMgr9::SetStencilFailOp(DRSStencilOp stencilFail)
{
	if (m_device == NULL)
		return;
	if (m_stencilFail == stencilFail)
		return;
	ChangeStencilFailOp(stencilFail);
}

void DRenderStateMgr9::SetStencilZFailOp(DRSStencilOp stencilZFail)
{
	if (m_device == NULL)
		return;
	if (m_stencilZFail == stencilZFail)
		return;
	ChangeStencilZFailOp(stencilZFail);
}

void DRenderStateMgr9::ChangeCullMode(DCullMode cullMode)
{
	if (cullMode == DCullMode_Back)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if(cullMode == DCullMode_Front)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	else if (cullMode == DCullMode_Off)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_currentCullMode = cullMode;
}

void DRenderStateMgr9::ChangeFillMode(DFillMode fillMode)
{
	if (fillMode == DFillMode_Solid)
		m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	else if (fillMode == DFillMode_WireFrame)
		m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_currentFillMode = fillMode;
}

void DRenderStateMgr9::ChangeZWrite(bool zwrite)
{
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, zwrite);
	m_zwrite = zwrite;
}

void DRenderStateMgr9::ChangeZTest(DRSCompareFunc ztest)
{
	
	m_device->SetRenderState(D3DRS_ZFUNC, GetComparisonFunc(ztest));
	m_ztest = ztest;
}

void DRenderStateMgr9::ChangeBlendEnable(bool enable)
{
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, enable);
	m_enableBlend = enable;
}

void DRenderStateMgr9::ChangeBlendOp(DRSBlendOp op)
{
	m_device->SetRenderState(D3DRS_BLENDOP, GetBlendOp(op));
	m_blendOp = op;
}

void DRenderStateMgr9::ChangeBlendSrcFactor(DRSBlendFactor factor)
{
	m_device->SetRenderState(D3DRS_SRCBLEND, GetBlendFactor(factor));
	m_blendSrcFactor = factor;
}

void DRenderStateMgr9::ChangeBlendDstFactor(DRSBlendFactor factor)
{
	m_device->SetRenderState(D3DRS_DESTBLEND, GetBlendFactor(factor));
	m_blendDstFactor = factor;
}

void DRenderStateMgr9::ChangeStencilEnable(bool enable)
{
	m_device->SetRenderState(D3DRS_STENCILENABLE, enable);
	m_enableStencil = enable;
}

void DRenderStateMgr9::ChangeStencilId(unsigned short stencilId)
{
	m_device->SetRenderState(D3DRS_STENCILREF, stencilId);
	m_stencilId = stencilId;
}

void DRenderStateMgr9::ChangeStencilCompFunc(DRSCompareFunc comp)
{
	m_device->SetRenderState(D3DRS_STENCILFUNC, GetComparisonFunc(comp));
	m_stencilComp = comp;
}

void DRenderStateMgr9::ChangeStencilPassOp(DRSStencilOp op)
{
	m_device->SetRenderState(D3DRS_STENCILPASS, GetStencilOp(op));
	m_stencilPass = op;
}

void DRenderStateMgr9::ChangeStencilFailOp(DRSStencilOp op)
{
	m_device->SetRenderState(D3DRS_STENCILFAIL, GetStencilOp(op));
	m_stencilFail = op;
}

void DRenderStateMgr9::ChangeStencilZFailOp(DRSStencilOp op)
{
	m_device->SetRenderState(D3DRS_STENCILZFAIL, GetStencilOp(op));
	m_stencilZFail = op;
}

void DRenderStateMgr9::ChangeStencilReadMask(unsigned short readMask)
{
	m_device->SetRenderState(D3DRS_STENCILMASK, readMask);
	m_stencilReadMask = readMask;
}

void DRenderStateMgr9::ChangeStencilWriteMask(unsigned short writeMask)
{
	m_device->SetRenderState(D3DRS_STENCILWRITEMASK, writeMask);
	m_stencilWriteMask = writeMask;
}

D3DCMPFUNC DRenderStateMgr9::GetComparisonFunc(DRSCompareFunc func)
{
	switch (func)
	{
	case DRSCompareFunc_Always:
		return D3DCMP_ALWAYS;
	case DRSCompareFunc_Equal:
		return D3DCMP_EQUAL;
	case DRSCompareFunc_GEqual:
		return D3DCMP_GREATEREQUAL;
	case DRSCompareFunc_Greater:
		return D3DCMP_GREATER;
	case DRSCompareFunc_LEqual:
		return D3DCMP_LESSEQUAL;
	case DRSCompareFunc_Less:
		return D3DCMP_LESS;
	case DRSCompareFunc_Never:
		return D3DCMP_NEVER;
	case DRSCompareFunc_NotEqual:
		return D3DCMP_NOTEQUAL;
	default:
		return D3DCMP_LESSEQUAL;
	}
}

D3DBLENDOP DRenderStateMgr9::GetBlendOp(DRSBlendOp op)
{
	switch (op)
	{
	case DRSBlendOp_Add:
		return D3DBLENDOP_ADD;
	case DRSBlendOp_Sub:
		return D3DBLENDOP_SUBTRACT;
	case DRSBlendOp_RevSub:
		return D3DBLENDOP_REVSUBTRACT;
	case DRSBlendOp_Min:
		return D3DBLENDOP_MIN;
	case DRSBlendOp_Max:
		return D3DBLENDOP_MAX;
	default:
		return D3DBLENDOP_ADD;
	}
}

D3DBLEND DRenderStateMgr9::GetBlendFactor(DRSBlendFactor factor)
{
	switch (factor)
	{
	case DRSBlendFactor_One:
		return D3DBLEND_ONE;
	case DRSBlendFactor_Zero:
		return D3DBLEND_ZERO;
	case DRSBlendFactor_SrcColor:
		return D3DBLEND_SRCCOLOR;
	case DRSBlendFactor_SrcAlpha:
		return D3DBLEND_SRCALPHA;
	case DRSBlendFactor_DstColor:
		return D3DBLEND_DESTCOLOR;
	case DRSBlendFactor_DstAlpha:
		return D3DBLEND_DESTALPHA;
	case DRSBlendFactor_OneMinusSrcColor:
		return D3DBLEND_INVSRCCOLOR;
	case DRSBlendFactor_OneMinusSrcAlpha:
		return D3DBLEND_INVSRCALPHA;
	case DRSBlendFactor_OneMinusDstColor:
		return D3DBLEND_INVDESTCOLOR;
	case DRSBlendFactor_OneMinusDstAlpha:
		return D3DBLEND_INVDESTALPHA;
	default:
		return D3DBLEND_ZERO;
	}
}

D3DSTENCILOP DRenderStateMgr9::GetStencilOp(DRSStencilOp op)
{
	switch (op)
	{
	case DRSStencilOp_Keep:
		return D3DSTENCILOP_KEEP;
	case DRSStencilOp_Zero:
		return D3DSTENCILOP_ZERO;
	case DRSStencilOp_Replace:
		return D3DSTENCILOP_REPLACE;
	case DRSStencilOp_IncrementSaturate:
		return D3DSTENCILOP_INCRSAT;
	case DRSStencilOp_DecrementSaturate:
		return D3DSTENCILOP_DECRSAT;
	case DRSStencilOp_Invert:
		return D3DSTENCILOP_INVERT;
	case DRSStencilOp_IncrementWrap:
		return D3DSTENCILOP_INCR;
	case DRSStencilOp_DecrementWrap:
		return D3DSTENCILOP_DECR;
	default:
		return D3DSTENCILOP_KEEP;
	}
}
#endif