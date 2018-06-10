#pragma once
#include "DGLCore.h"
#include "DMath.h"
#include <d3d9.h>
#include <d3dx9.h>

class D3D9Core : public DGLCore
{
public:
	D3D9Core();
	~D3D9Core();

	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender();
	virtual void EndRender();
	virtual DMeshRes* CreateMeshRes();
	virtual DTextureRes* CreateTextureRes();
	virtual DShaderRes* CreateShaderRes();

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return m_device;
	}

private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_device;
	D3DVIEWPORT9 m_viewPort;
};

