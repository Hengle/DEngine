#pragma once
#ifdef _DGAPI_D3D11

#include "D3D11Core.h"

/*
	纹理包装器Direct3D11底层
*/
class DTextureWrapper11 : public ITextureWrapper
{
public:
	DTextureWrapper11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, WCHAR* filename);
	DTextureWrapper11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, DTextureWrapper11*, DTextureWrapper11*, DTextureWrapper11*, DTextureWrapper11*, DTextureWrapper11*, DTextureWrapper11*);
	~DTextureWrapper11();
	virtual void Apply(UINT, int);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();

protected:
	ID3D11ShaderResourceView* m_texture;
	bool m_isSuccess;
	ID3D11DeviceContext* m_deviceContext;
};

/*
	RenderTexture Direct3D11底层
*/
class DRenderTextureViewWrapper11 : public IRenderTextureViewWrapper
{
public:
	DRenderTextureViewWrapper11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, float, float);
	DRenderTextureViewWrapper11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, ID3D11Texture2D* backbuffer, ID3D11Texture2D* depthbuffer);
	~DRenderTextureViewWrapper11();
	virtual void Apply(UINT, int);
	virtual void ApplyWrapMode(UINT, DWrapMode);
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

#endif