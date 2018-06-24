#include "DTextureRes9.h"
#include "DSystem.h"

DTextureRes9::DTextureRes9(LPDIRECT3DDEVICE9 device, WCHAR * filename)
{
	m_device = device;

	HRESULT result = D3DXCreateTextureFromFile(device, filename, &m_texture);
	m_isSuccess = false;
	if (FAILED(result))
	{
		return;
	}
	m_isSuccess = true;
}

DTextureRes9::~DTextureRes9()
{
}

void DTextureRes9::Apply(UINT textureOffset, DWrapMode mode)
{
	if (m_isSuccess)
	{
		m_device->SetTexture(textureOffset, m_texture);
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplySamplerState(textureOffset, mode);
	}
}

void DTextureRes9::Release()
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}
	m_device = NULL;
}
