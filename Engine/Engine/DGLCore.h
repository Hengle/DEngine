#pragma once
#include "DColor.h"
#include "DMath.h"
#include "DGraphicsDefine.h"
#include "DEngineDefine.h"
#include "DShaderPass.h"
#include <d3dcommon.h>
#include <string>
#include <map>

/*shader属性描述*/
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
	int propertySize;//属性占用大小
	bool isGlobal;//是否为全局属性
	std::string propertyName;//属性名称
} DShaderPropertyDesc;

/*shader ConstantBuffer描述*/
class DShaderCBufferDesc
{
public:
	DShaderCBufferDesc()
	{
		cbufferIndex = -1;
		cbufferStartSlot = -1;
		cbufferSize = 0;
	}

public:
	int cbufferIndex;//用于索引该cbuffer指针
	int cbufferStartSlot;
	int cbufferSize;//cbuffer大小
	std::map<std::string, DShaderPropertyDesc> properties;
};

/*geometrybuffer描述*/
typedef struct DGeometryBufferDesc
{
public:
	DGeometryBufferDesc(){}
public:
	int vertexCount /*顶点数量*/, indexCount /*索引数量*/;
	float*vertices; /*顶点缓存*/
	float*uvs; 
	float*uv2s; 
	float*uv3s;
	float*normals;
	float*tangents;
	float*colors;
	unsigned int*indices;
} DGeometryBufferDesc;

//抽象Geometry包装器-用于实现不同API下的Geometry
class DGeometryWrapper
{
public:
	DGeometryWrapper(int vertexUsage /*顶点用法描述*/, bool dynamic /*是否为动态mesh*/);
	void Refresh(DGeometryBufferDesc* desc); //更新顶点缓存
	void Refresh(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount);//更新顶点缓存
	void DrawPrimitive(DGeometryTopology topology);//绘制
	virtual void Release() = 0; //释放资源
	//bool IsInitialized();
	bool IsDirty();
	void SetDirty();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount) = 0;
	virtual bool OnInit(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount) = 0;
	virtual void OnDraw(DGeometryTopology) = 0;

protected:
	int m_vertexUsage;
	int m_uv0Offset;
	int m_uv1Offset;
	int m_uv2Offset;
	int m_uv3Offset;
	int m_colorOffset;
	int m_normalOffset;
	int m_tangentOffset;
	int m_binormalOffset;
	int m_uv0Layout;
	int m_uv1Layout;
	int m_uv2Layout;
	int m_uv3Layout;
	int m_colorLayout;
	int m_normalLayout;
	int m_tangentLayout;
	int m_binormalLayout;
	int m_indexCount;
	int m_vertexCount;
	int m_dataCount;
	bool m_isDynamic;
	int m_dataSize;

private:
	bool m_isInitialized;
	bool m_isSupported;
	bool m_isDirty;
};

/*抽象贴图包装器接口-用于实现不同API下的texture*/
interface ITextureWrapper
{
public:
	virtual void Release() = 0;
	virtual void Apply(UINT, int) = 0;
	virtual void ApplyWrapMode(UINT, DWrapMode) = 0;
};

/*抽象RenderBuffer接口*/
interface IRenderBuffer
{
public:
	virtual void Release() = 0;
};

/*抽象RenderTextureView包装器接口*/
interface IRenderTextureViewWrapper : public ITextureWrapper
{
public:

	virtual IRenderBuffer* GetColorBuffer() = 0;
	virtual IRenderBuffer* GetDepthBuffer() = 0;
};

/*抽象状态管理器接口*/
interface IRenderStateMgr
{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void SetCullMode(DCullMode) = 0;
	virtual void SetFillMode(DFillMode) = 0;
	virtual void SetZWriteEnable(bool) = 0;
	virtual void SetZTestFunc(DRSCompareFunc) = 0;
	virtual void SetBlendOp(DRSBlendOp) = 0;
	virtual void SetBlendEnable(bool) = 0;
	virtual void SetBlendSrcFactor(DRSBlendFactor) = 0;
	virtual void SetBlendDstFactor(DRSBlendFactor) = 0;
	virtual void SetBlendFactor(DRSBlendFactor, DRSBlendFactor) = 0;
	virtual void SetStencilRefId(UINT) = 0;
	virtual void SetStencilEnable(bool) = 0;
	virtual void SetStencilReadMask(unsigned short) = 0;
	virtual void SetStencilWriteMask(unsigned short) = 0;
	virtual void SetStencilComparisonFunc(DRSCompareFunc) = 0;
	virtual void SetStencilOp(DRSStencilOp stencilFail, DRSStencilOp zFail, DRSStencilOp stencilPass) = 0;
	virtual void SetStencilPassOp(DRSStencilOp) = 0;
	virtual void SetStencilFailOp(DRSStencilOp) = 0;
	virtual void SetStencilZFailOp(DRSStencilOp) = 0;
};

/*抽象图形库模块*/
class DGLCore
{
public:
	DGLCore();
	~DGLCore();
	/*初始化图形库*/
	bool Init(int width, int height, bool fullscreen);
	/*销毁图形库*/
	virtual void Destroy() = 0;
	/*提交渲染结果*/
	virtual void Present() = 0;
	/*清除缓冲区*/
	virtual void Clear(bool, bool, bool, DColor&, IRenderTextureViewWrapper* = NULL) = 0;
	/*设置渲染目标*/
	virtual void SetRenderTarget(IRenderTextureViewWrapper* = NULL) = 0;
	/*设置视口区域*/
	virtual void SetViewPort(float, float, float, float) = 0;
	/*结束渲染*/
	virtual void EndSetRenderTarget(IRenderTextureViewWrapper* = NULL) = 0;
	/*创建几何体包装器*/
	virtual DGeometryWrapper* CreateGeometryWrapper(int, bool) = 0;
	/*创建贴图包装器*/
	virtual ITextureWrapper* CreateTextureWrapper(WCHAR*) = 0;
	/*创建RenderTexture包装器*/
	virtual IRenderTextureViewWrapper* CreateRenderTextureWrapper(float, float) = 0;

	/*创建CubeMap包装器*/
	virtual ITextureWrapper* CreateCubeMapWrapper(ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*) = 0;
	/*创建shader程序*/
	virtual DShaderPass* CreateShaderPass() = 0;
	virtual void ApplySamplerState(UINT, DWrapMode) = 0;
	/*获取渲染状态管理器*/
	virtual IRenderStateMgr* GetRenderStateMgr() = 0;

	virtual bool IsFrustrumZeroToOne() = 0;
	virtual bool IsUVStartsAtTop() = 0;
	/*获取屏幕分辨率*/
	void GetResolution(float&, float&);

protected:

	float m_width;
	float m_height;
};