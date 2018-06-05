#pragma once
#include "DGLCore.h"

class DMeshBuffer9 : public DMeshBuffer
{
public:
	virtual void Release();
};

class DTextureBuffer9 : public DTextureBuffer
{
public:
	virtual void Release();
};

class DShaderBuffer9 : public DShaderBuffer
{
public:
	virtual unsigned int GetCBufferCount() const;
	virtual unsigned int GetPropertyCount() const;
	virtual void GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int& poffset, int& plength, int& stype) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();
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
};

