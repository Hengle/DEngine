#include "DRenderTexture.h"

DRenderTexture::DRenderTexture(int width, int height)
{
	m_width = width;
	m_height = height;
	m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
}

DRenderTexture::~DRenderTexture()
{
}

bool DRenderTexture::Init(ID3D11Device * device)
{
	return false;
}

void DRenderTexture::Destroy()
{
}

void DRenderTexture::SetRenderTarget(ID3D11DeviceContext *, ID3D11DepthStencilView *)
{
}

void DRenderTexture::ClearRenderTarget(ID3D11DeviceContext *, ID3D11DepthStencilView *, float, float, float, float)
{
}

ID3D11ShaderResourceView * DRenderTexture::GetTexture()
{
	return nullptr;
}

int DRenderTexture::GetWidth()
{
	return 0;
}

int DRenderTexture::GetHeight()
{
	return 0;
}
