#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

class MyTestShader
{
public:
	MyTestShader();
	~MyTestShader();
	void Init(LPDIRECT3DDEVICE9, WCHAR * vsfile, WCHAR* psfile);
	void Release();
	void SetWorldMatrix(LPDIRECT3DDEVICE9 device, D3DXMATRIX* matrix);
	void SetViewMatrix(LPDIRECT3DDEVICE9 device, D3DXMATRIX* matrix);
	void SetProjMatrix(LPDIRECT3DDEVICE9 device, D3DXMATRIX* matrix);
	void Draw(LPDIRECT3DDEVICE9 device);

//private:
//	HRESULT InitVertexShader();

private:
	IDirect3DVertexShader9* m_vertexShader;
	IDirect3DPixelShader9* m_pixelShader;
	ID3DXConstantTable* m_vertexConstable;
	ID3DXConstantTable* m_pixelConstable;
	D3DXHANDLE m_worldMatrix;
	D3DXHANDLE m_viewMatrix;
	D3DXHANDLE m_projMatrix;
};

