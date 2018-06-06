#pragma once
#include "DGLCore.h"
#include <d3d9.h>
#include <d3dx9.h>

class DMeshBuffer9 : public DMeshBuffer
{
public:
	DMeshBuffer9();
	void Init(LPDIRECT3DDEVICE9, int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices);
	virtual void Release();

private:
	IDirect3DVertexBuffer9* m_vertexBuffer;
	IDirect3DIndexBuffer9* m_indexBuffer;
};

class DTextureBuffer9 : public DTextureBuffer
{
public:
	virtual void Release();
};

class DShaderBuffer9 : public DShaderBuffer
{
public:
	DShaderBuffer9();
	~DShaderBuffer9();
	void Init(LPDIRECT3DDEVICE9, WCHAR * vertexShader, WCHAR * pixelShader);
	virtual unsigned int GetCBufferCount() const;
	virtual unsigned int GetPropertyCount() const;
	virtual void GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int& poffset, int& plength, int& stype) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

private:
	bool InitShader(LPDIRECT3DDEVICE9, WCHAR*, WCHAR*);
	HRESULT InitVertexShader(ID3DBlob*, LPDIRECT3DDEVICE9, ID3D11InputLayout**, int*);
	HRESULT InitPixelShader(ID3DBlob*, LPDIRECT3DDEVICE9);

private:
	IDirect3DVertexShader9* m_vertexShader;
	IDirect3DPixelShader9* m_pixelShader;
	ID3DXConstantTable* m_vertexConstable;
	ID3DXConstantTable* m_pixelConstable;
};

class D3D9Core : public DGLCore
{
public:
	D3D9Core();
	~D3D9Core();

	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender(float, float, float, float);
	virtual void EndRender();
	virtual DMeshBuffer* CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices);
	virtual DTextureBuffer* CreateTextureBuffer(WCHAR* fileName);
	virtual DShaderBuffer* CreateShaderBuffer(WCHAR* vertexShader, WCHAR* pixelShader);
	virtual void ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float* params);
	virtual void ApplyTextureParams(DTextureBuffer* textureBuffer);
	virtual void DrawMesh(const DMeshBuffer*, int);
	virtual void DrawShader(const DShaderBuffer*, int);

	virtual void SetBackBufferRenderTarget();

private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_device;
};

