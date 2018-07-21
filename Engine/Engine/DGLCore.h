#pragma once
#include "DColor.h"
#include "DMath.h"
#include "DGraphicsDefine.h"
#include "DEngineDefine.h"
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

/*shader资源描述*/
typedef struct DShaderResDesc
{
public:
	DShaderResDesc()
	{
		offset = 0;
		isGlobal = false;
	}

public:
	UINT offset;//资源偏移
	std::string resName;//资源名称
	bool isGlobal;//是否为全局属性
};

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
	float*colors;
	unsigned long*indices;
} DGeometryBufferDesc;

//抽象Geometry资源-用于实现不同API下的Geometry
class DGeometryRes
{
public:
	DGeometryRes(int vertexUsage /*顶点用法描述*/, bool dynamic /*是否为动态mesh*/);
	void Refresh(DGeometryBufferDesc* desc); //更新顶点缓存
	void Refresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);//更新顶点缓存
	void DrawPrimitive(DGeometryTopology topology);//绘制
	virtual void Release() = 0; //释放资源
	bool IsInitialized();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
	virtual void OnDraw(DGeometryTopology) = 0;

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

/*抽象贴图资源接口-用于实现不同API下的texture*/
interface ITextureRes
{
public:
	virtual void Release() = 0;
	virtual void Apply(UINT, DWrapMode) = 0;
};

/*抽象RenderBuffer接口*/
interface IRenderBuffer
{
public:
	virtual void Release() = 0;
};

/*抽象RenderTextureView接口*/
interface IRenderTextureViewRes
{
public:
	virtual void Release() = 0;
	virtual void Apply(UINT, DWrapMode) = 0;

	virtual IRenderBuffer* GetColorBuffer() = 0;
	virtual IRenderBuffer* GetDepthBuffer() = 0;
};

//抽象shader program-用于实现不同API下的不同shader
class DShaderProgram
{
public:
	DShaderProgram();
	/*获得属性数量*/
	unsigned int GetPropertyCount() const;
	/*初始化*/
	void Init(const char* content /*shader内容*/, char* funcName /*函数名*/);
	/*获得shader资源属性数量*/
	unsigned int GetResCount() const; 
	/*应用shader参数*/
	void ApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	void Draw();
	bool IsInitialized();
	int GetVertexUsage();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const = 0;
	virtual bool HasProperty(const LPCSTR key) const = 0;
	virtual void Release() = 0;

protected:
	virtual bool OnInit(const char* content, char* funcName) = 0;
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams) = 0;
	virtual void OnDraw() = 0;

protected:
	unsigned int m_cbufferCount, m_propertyCount, m_resCount;
	bool m_isInitialized;
	int m_vertexUsage;
};

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
	virtual void SetStencilRefId(UINT) = 0;
	virtual void SetStencilEnable(bool) = 0;
	virtual void SetStencilReadMask(unsigned short) = 0;
	virtual void SetStencilWriteMask(unsigned short) = 0;
	virtual void SetStencilComparisonFunc(DRSCompareFunc) = 0;
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
	virtual bool Init(int width, int height, bool fullscreen, HWND);
	/*销毁图形库*/
	virtual void Destroy() = 0;
	/*提交渲染结果*/
	virtual void Present() = 0;
	/*清除缓冲区*/
	virtual void Clear(bool, bool, bool, DColor&, IRenderTextureViewRes* = NULL) = 0;
	/*设置渲染目标*/
	virtual void SetRenderTarget(IRenderTextureViewRes* = NULL) = 0;
	/*设置视口区域*/
	virtual void SetViewPort(float, float, float, float) = 0;
	/*结束渲染*/
	virtual void EndSetRenderTarget(IRenderTextureViewRes* = NULL) = 0;
	/*创建几何体资源*/
	virtual DGeometryRes* CreateGeometryRes(int, bool) = 0;
	/*创建贴图资源*/
	virtual ITextureRes* CreateTextureRes(WCHAR*) = 0;
	/*创建RenderTexture资源*/
	virtual IRenderTextureViewRes* CreateRenderTextureRes(float, float) = 0;
	/*创建shader程序*/
	virtual DShaderProgram* CreateShaderProgram(DShaderProgramType) = 0;
	virtual void ApplySamplerState(UINT, DWrapMode) = 0;
	/*获取渲染状态管理器*/
	virtual IRenderStateMgr* GetRenderStateMgr() = 0;
	/*获取屏幕分辨率*/
	void GetResolution(float&, float&);

protected:

	float m_width;
	float m_height;
};