#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h>

class DMeshBuffer
{
public:
	virtual void Release() = 0;
};

class DTextureBuffer
{
public:
	virtual void Release() = 0;
};

class DShaderBuffer
{
public:
	virtual unsigned int GetCBufferCount() const = 0;
	virtual unsigned int GetCBufferIndex(LPCSTR cbuffername) const = 0;
	virtual void Release() = 0;
};

class DGLCore
{
public:
	DGLCore();
	~DGLCore();
	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy() = 0;
	virtual void BeginRender(float, float, float, float) = 0;
	virtual void EndRender() = 0;
	virtual DMeshBuffer* CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices) = 0;
	virtual DTextureBuffer* CreateTextureBuffer(WCHAR* fileName) = 0;
	virtual DShaderBuffer* CreateShaderBuffer(WCHAR* vertexShader, WCHAR* pixelShader) = 0;
	virtual void DrawMesh(const DMeshBuffer*, int) = 0;

	virtual void SetBackBufferRenderTarget() = 0;
	void GetResolution(FLOAT&, FLOAT&);

protected:

	FLOAT m_width;
	FLOAT m_height;
};