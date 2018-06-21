#pragma once
#include <d3dcommon.h>

typedef struct DShaderResDesc
{
public:
	DShaderResDesc()
	{

	}
	
public:

} DShaderResDesc;

typedef struct DShaderParamDesc
{
public:
	DShaderParamDesc()
	{
		cbufferIndex = -1;
		cbufferOffset = -1;
		cbufferLength = 0;
		propertyOffset = 0;
		propertySize = 0;
		shaderType = 0;
	}

public:
	int cbufferIndex, cbufferOffset, cbufferLength,propertyOffset, propertySize,shaderType;
} DShaderParamDesc;

typedef struct DMeshBufferDesc
{
public:
	DMeshBufferDesc(){}
public:
	int vertexCount, indexCount, dataSize, dataCount;
	float*vertices;
	unsigned long*indices;
} DMeshBufferDesc;

//抽象Mesh资源-用于实现不同API下的mesh
class DMeshRes
{
public:
	DMeshRes();
	void Init(DMeshBufferDesc* desc);
	void Draw();
	virtual void Release() = 0;
	bool IsInitialized();

protected:
	virtual bool OnInit(DMeshBufferDesc*) = 0;
	virtual void OnDraw() = 0;

private:
	bool m_isInitialized;
};

//抽象贴图资源-用于实现不同API下的texture
class DTextureRes
{
public:
	virtual void Release() = 0;
};

//抽象shader资源-用于实现不同API下的shader
class DShaderRes
{
public:
	DShaderRes();
	unsigned int GetCBufferCount() const;
	unsigned int GetPropertyCount() const;
	void Init(WCHAR* vsfile, WCHAR* psfile);
	void ApplyParams(int cindex, int coffset, int csize, int stype, float* params);
	void ApplyRes(DTextureRes*);
	void Draw();
	bool IsInitialized();
	virtual void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const = 0;
	virtual bool HasProperty(const LPCSTR key) const = 0;
	virtual void Release() = 0;

protected:
	virtual bool OnInit(WCHAR*, WCHAR*) = 0;
	virtual void OnApplyParams(int, int, int, int, float*) = 0;
	virtual void OnApplyRes(DTextureRes*) = 0;
	virtual void OnDraw() = 0;

protected:
	unsigned int m_cbufferCount, m_propertyCount;
	bool m_isInitialized;
};

class DGLCore
{
public:
	DGLCore();
	~DGLCore();
	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy() = 0;
	virtual void BeginRender() = 0;
	virtual void EndRender() = 0;
	virtual DMeshRes* CreateMeshRes() = 0;
	virtual DTextureRes* CreateTextureRes() = 0;
	virtual DShaderRes* CreateShaderRes() = 0;
	void GetResolution(float&, float&);

protected:

	float m_width;
	float m_height;
};