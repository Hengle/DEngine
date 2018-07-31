#pragma once
#ifdef _DGAPI_D3D10

#include "D3D10Core.h"

/*
	纹理资源Direct3D10底层
*/
class DTextureRes10 : public ITextureRes
{
public:
	DTextureRes10(ID3D10Device* device, WCHAR* filename);
	~DTextureRes10();
	virtual void Apply(UINT);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();

private:
	ID3D10ShaderResourceView* m_texture;
	bool m_isSuccess;
	ID3D10Device* m_device;
};

/*
	RenderTexture Direct3D10底层
*/
class DRenderTextureViewRes10 : public IRenderTextureViewRes
{
public:
	DRenderTextureViewRes10(ID3D10Device* device, float, float);
	~DRenderTextureViewRes10();
	virtual void Apply(UINT);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();
	virtual IRenderBuffer* GetColorBuffer();
	virtual IRenderBuffer* GetDepthBuffer();

private:
	ID3D10ShaderResourceView* m_texture;
	ID3D10Texture2D* m_renderTexture;
	ID3D10Texture2D* m_depthTexture;
	bool m_isSuccess;
	ID3D10Device* m_device;
	IRenderBuffer* m_colorBuffer;
	IRenderBuffer* m_depthBuffer;
};


#endif