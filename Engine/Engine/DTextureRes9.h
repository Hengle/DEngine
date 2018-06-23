#pragma once
#include "D3D9Core.h"

class DTextureRes9 : public DTextureRes
{
public:
	DTextureRes9(LPDIRECT3DDEVICE9 device, WCHAR* filename);
	~DTextureRes9();
	virtual void Apply(UINT, DWarpMode);
	virtual void Release();

private:
	IDirect3DTexture9* m_texture;
	bool m_isSuccess;
	LPDIRECT3DDEVICE9 m_device;
};

