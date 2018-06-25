#pragma once
#include <d3dcommon.h>

#define	DVertexStructure_NONE        0
#define	DVertexStructure_POSITION    1
#define	DVertexStructure_TEXCOORD0   2
#define	DVertexStructure_TEXCOORD1   4
#define	DVertexStructure_TEXCOORD2   8
#define	DVertexStructure_TEXCOORD3   16
#define	DVertexStructure_COLOR       32
#define	DVertexStructure_NORMAL      64
#define	DVertexStructure_TANGENT     128
#define	DVertexStructure_BINORMAL    256

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
	void Draw(DMeshTopology);
	virtual void Release() = 0;
	bool IsInitialized();

protected:
	virtual bool OnInit(DMeshBufferDesc*) = 0;
	virtual void OnDraw(DMeshTopology) = 0;

private:
	bool m_isInitialized;
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

};

class DRenderTextureViewRes
{
public:
	virtual void Release() = 0;
	virtual void Apply(UINT, DWrapMode) = 0;
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
	void Draw();
	bool IsInitialized();
	virtual void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const = 0;
	virtual UINT GetResOffset(const LPCSTR key) const = 0;
	virtual bool HasProperty(const LPCSTR key) const = 0;
	virtual void Release() = 0;

protected:
	virtual bool OnInit(WCHAR*, WCHAR*) = 0;
	virtual void OnApplyParams(int, int, int, int, float*) = 0;
	virtual void OnDraw() = 0;

protected:
	unsigned int m_cbufferCount, m_propertyCount;
	bool m_isInitialized;
	int m_vertexStructure;
};

class DGLDrawer
{
//public:
//	virtual void glBegin() = 0;
//	virtual void glEnd() = 0;
//	virtual void glVector3(DVector3*) = 0;
//	virtual void glColor(DColor*) = 0;
//
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
	virtual DMeshRes* CreateMeshRes() = 0;
	virtual DTextureRes* CreateTextureRes(WCHAR*) = 0;
	virtual DShaderRes* CreateShaderRes() = 0;
	virtual void ApplySamplerState(UINT, DWrapMode) = 0;
	virtual DRenderStateMgr* GetRenderStateMgr() = 0;
	//virtual void 
	void GetResolution(float&, float&);

protected:

	float m_width;
	float m_height;
};