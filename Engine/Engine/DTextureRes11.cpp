#include "DTextureRes11.h"
#include "DSystem.h"

DTextureRes11::DTextureRes11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, WCHAR* filename)
{
	HRESULT result;

	m_deviceContext = deviceContext;

	m_isSuccess = false;

	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return;
	}
	m_isSuccess = true;
}

DTextureRes11::~DTextureRes11()
{
}

void DTextureRes11::Apply(UINT textureOffset, DWarpMode mode)
{
	if (m_isSuccess)
	{
		m_deviceContext->PSSetShaderResources(textureOffset, 1, &m_texture);
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplySamplerState(textureOffset, mode);
	}
}

void DTextureRes11::Release()
{
	if (m_texture != NULL)
		m_texture->Release();
	m_texture = NULL;
}
