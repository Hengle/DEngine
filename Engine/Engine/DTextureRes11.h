#pragma once
#include "D3D11Core.h"

/*
	纹理资源Direct3D11底层
*/
class DTextureRes11 : public ITextureRes
{
public:
	DTextureRes11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, WCHAR* filename);
	~DTextureRes11();
	virtual void Apply(UINT, DWrapMode);
	virtual void Release();

private:
	ID3D11ShaderResourceView* m_texture;
	bool m_isSuccess;
	ID3D11DeviceContext* m_deviceContext;
};

/*
	RenderTexture Direct3D11底层
*/
class DRenderTextureViewRes11 : public IRenderTextureViewRes
{
public:
	DRenderTextureViewRes11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, float, float);
	~DRenderTextureViewRes11();
	virtual void Apply(UINT, DWrapMode);
	virtual void Release();
	virtual IRenderBuffer* GetColorBuffer();
	virtual IRenderBuffer* GetDepthBuffer();

private:
	ID3D11ShaderResourceView* m_texture;
	ID3D11Texture2D* m_renderTexture;
	ID3D11Texture2D* m_depthTexture;
	bool m_isSuccess;
	ID3D11DeviceContext* m_deviceContext;
	IRenderBuffer* m_colorBuffer;
	IRenderBuffer* m_depthBuffer;
};

