#include "DRenderStateMgr9.h"

DRenderStateMgr9::DRenderStateMgr9(IDirect3DDevice9* device)
{
	m_device = device;
}

DRenderStateMgr9::~DRenderStateMgr9()
{
}

void DRenderStateMgr9::Init()
{
	if (m_device == NULL)
		return;
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
