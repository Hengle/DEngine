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

void DRenderStateMgr9::SetZWriteEnable(bool)
{
}

void DRenderStateMgr9::SetZTestFunc(DRSCompareFunc)
{
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
