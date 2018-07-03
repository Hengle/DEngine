#pragma once
#include "D3D9Core.h"

/*
	纹理资源Direct3D9底层
*/
class DTextureRes9 : public DTextureRes
{
public:
	DTextureRes9(LPDIRECT3DDEVICE9 device, WCHAR* filename);
	~DTextureRes9();
	virtual void Apply(UINT, DWrapMode);
	virtual void Release();

private:
	IDirect3DTexture9* m_texture;
	bool m_isSuccess;
	LPDIRECT3DDEVICE9 m_device;
};

/*
	RenderTexture Direct3D9底层
*/
class DRenderTextureViewRes9 : public DRenderTextureViewRes
{
public:
	DRenderTextureViewRes9(LPDIRECT3DDEVICE9 device, float, float);
	~DRenderTextureViewRes9();
	virtual void Apply(UINT, DWrapMode);
	virtual void Release();
	virtual DRenderBuffer* GetColorBuffer();
	virtual DRenderBuffer* GetDepthBuffer();
	LPD3DXRENDERTOSURFACE GetInterface() 
	{
		return m_interface;
	}
	LPDIRECT3DSURFACE9 GetSurface()
	{
		return m_surface;
	}

private:
	IDirect3DTexture9* m_texture;
	LPD3DXRENDERTOSURFACE m_interface;
	LPDIRECT3DSURFACE9 m_surface;
	bool m_isSuccess;
	LPDIRECT3DDEVICE9 m_device;
};