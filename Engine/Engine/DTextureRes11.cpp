#include "DTextureRes11.h"
#include "DSystem.h"
#include "DRenderBuffer11.h"

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

void DTextureRes11::Apply(UINT textureOffset, DWrapMode mode)
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
	m_deviceContext = NULL;
}

DRenderTextureViewRes11::DRenderTextureViewRes11(ID3D11Device * device, ID3D11DeviceContext * deviceContext, float width, float height)
{
	m_isSuccess = false;
	HRESULT result;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTexture);
	if (FAILED(result))
		return;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthTexture);
	if (FAILED(result))
		return;

	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	m_colorBuffer = DColorBuffer11::Create(device, m_renderTexture, &renderTargetViewDesc);
	m_depthBuffer = DDepthBuffer11::Create(device, m_depthTexture, &depthStencilViewDesc);

	result = device->CreateShaderResourceView(m_renderTexture, &shaderResourceViewDesc, &m_texture);
	if (FAILED(result))
		return;

	m_deviceContext = deviceContext;
	m_isSuccess = true;
}

DRenderTextureViewRes11::~DRenderTextureViewRes11()
{
}

void DRenderTextureViewRes11::Apply(UINT textureOffset, DWrapMode mode)
{
	if (m_isSuccess)
	{
		m_deviceContext->PSSetShaderResources(textureOffset, 1, &m_texture);
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplySamplerState(textureOffset, mode);
	}
}

void DRenderTextureViewRes11::Release()
{
	if (m_texture != NULL)
		m_texture->Release();
	if (m_depthTexture != NULL)
		m_depthTexture->Release();
	if (m_renderTexture != NULL)
		m_renderTexture->Release();
	if (m_colorBuffer != NULL)
	{
		m_colorBuffer->Release();
		delete m_colorBuffer;
		m_colorBuffer = NULL;
	}
	if (m_depthBuffer != NULL)
	{
		m_depthBuffer->Release();
		delete m_depthBuffer;
		m_depthBuffer = NULL;
	}
	m_texture = NULL;
	m_deviceContext = NULL;
}

DRenderBuffer * DRenderTextureViewRes11::GetColorBuffer()
{
	return m_colorBuffer;
}

DRenderBuffer * DRenderTextureViewRes11::GetDepthBuffer()
{
	return m_depthBuffer;
}
