#pragma once
#include "D3D10Core.h"

class DTextureRes10 : public DTextureRes
{
public:
	DTextureRes10(ID3D10Device* device, WCHAR* filename);
	~DTextureRes10();
	virtual void Apply(UINT, DWrapMode);
	virtual void Release();

private:
	ID3D10ShaderResourceView* m_texture;
	bool m_isSuccess;
	ID3D10Device* m_device;
};

class DRenderTextureViewRes10 : public DRenderTextureViewRes
{
public:
	DRenderTextureViewRes10(ID3D10Device* device, float, float);
	~DRenderTextureViewRes10();
	virtual void Apply(UINT, DWrapMode);
	virtual void Release();
	virtual DRenderBuffer* GetColorBuffer();
	virtual DRenderBuffer* GetDepthBuffer();

private:
	ID3D10ShaderResourceView* m_texture;
	ID3D10Texture2D* m_renderTexture;
	ID3D10Texture2D* m_depthTexture;
	bool m_isSuccess;
	ID3D10Device* m_device;
	DRenderBuffer* m_colorBuffer;
	DRenderBuffer* m_depthBuffer;
};


