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

//class DShaderParam
//{
//public:
//	virtual void BeginSetParam(int id, void** value) = 0;
//	virtual void EndSetParam(int id) = 0;
//	virtual void Release() = 0;
//};

class DShaderBuffer
{
public:
	virtual unsigned int GetCBufferCount() const = 0;
	virtual unsigned int GetPropertyCount(LPCSTR cbuffername) const = 0;
	virtual int GetCBufferIndex(LPCSTR cbuffername) const = 0;
	virtual int GetCBufferOffset(LPCSTR cbuffername) const = 0;
	virtual int GetCBufferLength(LPCSTR cbuffername) const = 0;
	virtual int GetCBufferType(LPCSTR cbuffername) const = 0;
	virtual int GetPropertyIndex(const LPCSTR cbufferName, const LPCSTR key) const = 0;
	virtual int GetPropertyOffset(const LPCSTR cbufferName, const LPCSTR key) const = 0;
	virtual int GetPropertyLength(const LPCSTR cbufferName, const LPCSTR key) const = 0;
	virtual void GetPropertyInfo(const LPCSTR cbufferName, const LPCSTR key, int&, int&, int&) const = 0;
	virtual void GetCBufferInfo(LPCSTR, int&, int&, int&, int&) const = 0;
	virtual void Release() = 0;
	//virtual DShaderParam* GetParams() const = 0;
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
	virtual void ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float* params) = 0;
	virtual void DrawMesh(const DMeshBuffer*, int) = 0;
	virtual void DrawShader(const DShaderBuffer*, int) = 0;

	virtual void SetBackBufferRenderTarget() = 0;
	void GetResolution(FLOAT&, FLOAT&);

protected:

	FLOAT m_width;
	FLOAT m_height;
};