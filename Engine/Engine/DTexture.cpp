#include "DTexture.h"


DTexture::DTexture(WCHAR* fileName)
{
	m_texture = 0;
	m_textureFile = fileName;
}


DTexture::~DTexture()
{
}

void DTexture::Destroy()
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

bool DTexture::Init(ID3D11Device *device)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, m_textureFile, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView * DTexture::GetTexture()
{
	return m_texture;
}
