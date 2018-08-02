#ifdef _DGAPI_D3D10
#include "DTextureRes10.h"
#include "DSystem.h"
#include "DRenderBuffer10.h"
#include <D3DX10.h>

DTextureRes10::DTextureRes10(ID3D10Device * device, WCHAR * filename)
{
	HRESULT result;

	m_device = device;

	m_isSuccess = false;

	result = D3DX10CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return;
	}
	m_isSuccess = true;
}

DTextureRes10::DTextureRes10(ID3D10Device * device, DTextureRes10 * right, DTextureRes10 * left, DTextureRes10 * top, DTextureRes10 * bottom, DTextureRes10 * front, DTextureRes10 * back)
{
	m_device = device;

	m_isSuccess = false;

	ID3D10Resource* srcTex[6];

	right->m_texture->GetResource(&srcTex[1]);
	left->m_texture->GetResource(&srcTex[0]);
	top->m_texture->GetResource(&srcTex[2]);
	bottom->m_texture->GetResource(&srcTex[3]);
	front->m_texture->GetResource(&srcTex[4]);
	back->m_texture->GetResource(&srcTex[5]);


	D3D10_TEXTURE2D_DESC texElementDesc;
	((ID3D10Texture2D*)srcTex[0])->GetDesc(&texElementDesc);

	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	ID3D10Texture2D* texArray = 0;
	if (FAILED(device->CreateTexture2D(&texArrayDesc, 0, &texArray)))
		return;

	D3D10_BOX sourceRegion;

	for (UINT x = 0; x < 6; x++)
	{
		for (UINT mipLevel = 0; mipLevel < texArrayDesc.MipLevels; mipLevel++)
		{
			sourceRegion.left = 0;
			sourceRegion.right = (texArrayDesc.Width >> mipLevel);
			sourceRegion.top = 0;
			sourceRegion.bottom = (texArrayDesc.Height >> mipLevel);
			sourceRegion.front = 0;
			sourceRegion.back = 1;

			if (sourceRegion.bottom == 0 || sourceRegion.right == 0)
				break;

			m_device->CopySubresourceRegion(texArray, D3D10CalcSubresource(mipLevel, x, texArrayDesc.MipLevels), 0, 0, 0, srcTex[x], mipLevel, &sourceRegion);
		}
	}

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	if (FAILED(device->CreateShaderResourceView(texArray, &viewDesc, &m_texture)))
		return;

	m_isSuccess = true;
}

DTextureRes10::~DTextureRes10()
{
}

void DTextureRes10::Apply(UINT textureOffset, int)
{
	if (m_isSuccess)
	{
		m_device->PSSetShaderResources(textureOffset, 1, &m_texture);
	}
}

void DTextureRes10::ApplyWrapMode(UINT textureOffset, DWrapMode mode)
{
	if (m_isSuccess)
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplySamplerState(textureOffset, mode);
	}
}

void DTextureRes10::Release()
{
	if (m_texture != NULL)
		m_texture->Release();
	m_texture = NULL;
	m_device = NULL;
}

DRenderTextureViewRes10::DRenderTextureViewRes10(ID3D10Device * device, float width, float height)
{
	m_isSuccess = false;
	HRESULT result;
	D3D10_TEXTURE2D_DESC textureDesc;
	D3D10_TEXTURE2D_DESC depthBufferDesc;
	D3D10_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D10_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D10_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
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
	depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthTexture);
	if (FAILED(result))
		return;

	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	m_colorBuffer = DColorBuffer10::Create(device, m_renderTexture, &renderTargetViewDesc);
	m_depthBuffer = DDepthBuffer10::Create(device, m_depthTexture, &depthStencilViewDesc);

	result = device->CreateShaderResourceView(m_renderTexture, &shaderResourceViewDesc, &m_texture);
	if (FAILED(result))
		return;

	m_device = device;
	m_isSuccess = true;
}

DRenderTextureViewRes10::~DRenderTextureViewRes10()
{
}

void DRenderTextureViewRes10::Apply(UINT textureOffset, int)
{
	if (m_isSuccess)
	{
		m_device->PSSetShaderResources(textureOffset, 1, &m_texture);
	}
}

void DRenderTextureViewRes10::ApplyWrapMode(UINT textureOffset, DWrapMode mode)
{
	if (m_isSuccess)
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplySamplerState(textureOffset, mode);
	}
}

void DRenderTextureViewRes10::Release()
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
	m_depthTexture = NULL;
	m_renderTexture = NULL;
	m_device = NULL;
}

IRenderBuffer * DRenderTextureViewRes10::GetColorBuffer()
{
	return m_colorBuffer;
}

IRenderBuffer * DRenderTextureViewRes10::GetDepthBuffer()
{
	return m_depthBuffer;
}
#endif