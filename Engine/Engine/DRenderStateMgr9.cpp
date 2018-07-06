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
	ChangeZTest(DRSCompareFunc_LEqual);
	ChangeZWrite(true);
	ChangeBlendEnable(false);
}

void DRenderStateMgr9::Release()
{
	m_device = NULL;
}

void DRenderStateMgr9::SetCullMode(DCullMode cullMode)
{
	if (m_device == NULL)
		return;
	if (m_currentMode == cullMode)
		return;
	ChangeCullMode(cullMode);
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

void DRenderStateMgr9::ChangeCullMode(DCullMode cullMode)
{
	if (cullMode == DCullMode_Back)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if(cullMode == DCullMode_Front)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	else if (cullMode == DCullMode_Off)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_currentMode = cullMode;
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
