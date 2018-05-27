#include "DTexture.h"
#include "DSystem.h"

DTexture::DTexture()
{
}

DTexture::~DTexture()
{
}

DTexture2D::DTexture2D(WCHAR * fileName) : DTexture()
{
	m_texture = 0;
	m_textureFile = fileName;
}

DTexture2D::~DTexture2D()
{
}

void DTexture2D::Destroy()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
	if (m_textureFile != NULL)
	{
		m_textureFile = NULL;
	}

	return;
}

bool DTexture2D::Init()
{
	HRESULT result;

	ID3D11Device* device = DSystem::GetGraphicsMgr()->GetDevice();

	result = D3DX11CreateShaderResourceViewFromFile(device, m_textureFile, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView * DTexture2D::GetTexture()
{
	return m_texture;
}
