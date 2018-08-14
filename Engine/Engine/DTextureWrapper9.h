#pragma once
#ifdef _DGAPI_D3D9

#include "D3D9Core.h"

/*
	纹理包装器Direct3D9底层
*/
class DTextureWrapper9 : public ITextureWrapper
{
public:
	DTextureWrapper9(LPDIRECT3DDEVICE9 device, WCHAR* filename);
	DTextureWrapper9(LPDIRECT3DDEVICE9 device);
	~DTextureWrapper9();
	virtual void Apply(UINT, int);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();

private:
	IDirect3DTexture9* m_texture;
	bool m_isSuccess;
	LPDIRECT3DDEVICE9 m_device;
};

class DCubeTextureWrapper9 : public ITextureWrapper
{
public:
	DCubeTextureWrapper9(LPDIRECT3DDEVICE9 device, DTextureWrapper9* right, DTextureWrapper9* left, DTextureWrapper9* top, DTextureWrapper9* bottom, DTextureWrapper9* front, DTextureWrapper9* back);

private:
	bool m_isSuccess;
	LPDIRECT3DDEVICE9 m_device;
};

/*
	RenderTexture Direct3D9底层
*/
class DRenderTextureViewWrapper9 : public IRenderTextureViewWrapper
{
public:
	DRenderTextureViewWrapper9(LPDIRECT3DDEVICE9 device, float, float);
	~DRenderTextureViewWrapper9();
	virtual void Apply(UINT, int);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();
	virtual IRenderBuffer* GetColorBuffer();
	virtual IRenderBuffer* GetDepthBuffer();
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

#endif