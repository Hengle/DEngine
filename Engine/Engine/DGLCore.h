#pragma once
#include "DColor.h"
#include "DMath.h"
#include <d3dcommon.h>
#include <string>
#include <map>

enum DVertexUsage
{
	DVertexUsage_POSITION     = 0,
	DVertexUsage_TEXCOORD0    = 1,
	DVertexUsage_TEXCOORD1    = 2,
	DVertexUsage_TEXCOORD2    = 3,
	DVertexUsage_TEXCOORD3    = 4,
	DVertexUsage_COLOR        = 5,
	DVertexUsage_NORMAL       = 6,
	DVertexUsage_TANGENT      = 7,
	DVertexUsage_BINORMAL     = 8,
};

enum DWrapMode
{
	DWrapMode_Repeat = 0,
	DWrapMode_Clamp = 1,
};

enum DMeshTopology
{
	DMeshTopology_PointList,
	DMeshTopology_LineStrip,
	DMeshTopology_LineList,
	DMeshTopology_TriangleStrip,
	DMeshTopology_TriangleList,
};

enum DCullMode
{
	DCullMode_Back,
	DCullMode_Front,
	DCullMode_Off,
};

enum DRSCompareFunc
{
	DRSCompareFunc_Never = 0,
	DRSCompareFunc_Less = 1,
	DRSCompareFunc_Equal = 2,
	DRSCompareFunc_LEqual = 3,
	DRSCompareFunc_Greater = 4,
	DRSCompareFunc_NotEqual = 5,
	DRSCompareFunc_GEqual = 6,
	DRSCompareFunc_Always = 7,
};

enum DRSStencilOp
{
	Keep = 0,
	Zero = 1,
	Replace = 2,
	IncrementSaturate = 3,
	DecrementSaturate = 4,
	Invert = 5,
	IncrementWrap = 6,
};

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

typedef struct DShaderPropertyDesc
{
public:
	DShaderPropertyDesc()
	{
		propertyOffset = -1;
		propertySize = 0;
		isGlobal = false;
	}

public:
	int propertyOffset;//属性在cbuffer中的偏移量
	int propertySize;//属性大小
	bool isGlobal;//是否为全局属性
	std::string propertyName;//属性名称
} DShaderPropertyDesc;

typedef struct DShaderResDesc
{
public:
	DShaderResDesc()
	{
		offset = 0;
		isGlobal = false;
	}

public:
	UINT offset;
	std::string resName;
	bool isGlobal;
};

class DShaderCBufferDesc
{
public:
	DShaderCBufferDesc()
	{
		cbufferIndex = -1;
		cbufferStartSlot = -1;
		cbufferSize = 0;
		shaderType = 0;
	}

public:
	int cbufferIndex;//用于索引该cbuffer指针
	int cbufferStartSlot;
	int cbufferSize;//cbuffer大小
	int shaderType;
	std::map<std::string, DShaderPropertyDesc> properties;
};

typedef struct DMeshBufferDesc
{
public:
	DMeshBufferDesc(){}
public:
	int vertexCount, indexCount;
	float*vertices;
	float*uvs;
	float*uv2s;
	float*uv3s;
	float*normals;
	float*colors;
	unsigned long*indices;
} DMeshBufferDesc;

//抽象Mesh资源-用于实现不同API下的mesh
class DMeshRes
{
public:
	DMeshRes(int, bool);
	//void Init(DMeshBufferDesc* desc);
	void Refresh(DMeshBufferDesc* desc);
	void Refresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	void Draw(DMeshTopology);
	virtual void Release() = 0;
	bool IsInitialized();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
	virtual void OnDraw(DMeshTopology) = 0;

protected:
	int m_vertexUsage;
	bool m_hasUV;
	bool m_hasUV1;
	bool m_hasUV2;
	bool m_hasUV3;
	bool m_hasColor;
	bool m_hasNormal;
	bool m_hasTangent;
	bool m_hasBinormal;
	int m_indexCount;
	int m_vertexCount;
	int m_dataCount;
	bool m_isDynamic;
	int m_dataSize;

private:
	bool m_isInitialized;
	bool m_isSupported;

};

//抽象贴图资源-用于实现不同API下的texture
class DTextureRes
{
public:
	virtual void Release() = 0;
	virtual void Apply(UINT, DWrapMode) = 0;
};

class DRenderBuffer
{
public:
	virtual void Release() = 0;
};

class DRenderTextureViewRes
{
public:
	virtual void Release() = 0;
	virtual void Apply(UINT, DWrapMode) = 0;

	virtual DRenderBuffer* GetColorBuffer() = 0;
	virtual DRenderBuffer* GetDepthBuffer() = 0;
};

//抽象shader资源-用于实现不同API下的shader
class DShaderRes
{
public:
	DShaderRes();
	//unsigned int GetCBufferCount() const;
	unsigned int GetPropertyCount() const;
	void Init(WCHAR* vsfile, WCHAR* psfile);
	unsigned int GetResCount() const;
	void ApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	//void ApplyParams(int cindex, int coffset, int csize, int stype, float* params);
	void Draw();
	bool IsInitialized();
	int GetVertexUsage();
	//virtual void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const = 0;
	//virtual UINT GetResOffset(const LPCSTR key) const = 0;
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const = 0;
	virtual bool HasProperty(const LPCSTR key) const = 0;
	virtual void Release() = 0;

protected:
	virtual bool OnInit(WCHAR*, WCHAR*) = 0;
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams) = 0;
	//virtual void OnApplyParams(int, int, int, int, float*) = 0;
	virtual void OnDraw() = 0;

protected:
	unsigned int m_cbufferCount, m_propertyCount, m_resCount;
	bool m_isInitialized;
	int m_vertexUsage;
};

class DRenderStateMgr
{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void SetCullMode(DCullMode) = 0;
	virtual void SetZWriteEnable(bool) = 0;
	virtual void SetZTestFunc(DRSCompareFunc) = 0;
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
	virtual void Clear(bool, bool, DColor&, DRenderTextureViewRes* = NULL) = 0;
	virtual void SetRenderTarget(DRenderTextureViewRes* = NULL) = 0;
	virtual void EndSetRenderTarget(DRenderTextureViewRes* = NULL) = 0;
	virtual DMeshRes* CreateMeshRes(int, bool) = 0;
	virtual DTextureRes* CreateTextureRes(WCHAR*) = 0;
	virtual DRenderTextureViewRes* CreateRenderTextureRes(float, float) = 0;
	virtual DShaderRes* CreateShaderRes() = 0;
	virtual void ApplySamplerState(UINT, DWrapMode) = 0;
	virtual DRenderStateMgr* GetRenderStateMgr() = 0;
	//virtual void 
	void GetResolution(float&, float&);

protected:

	float m_width;
	float m_height;
};