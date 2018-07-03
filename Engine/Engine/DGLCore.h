﻿#pragma once
#include "DColor.h"
#include "DMath.h"
#include "DGraphicsDefine.h"
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
		shaderType = 0;
	}

public:
	int cbufferIndex;//用于索引该cbuffer指针
	int cbufferStartSlot;
	int cbufferSize;//cbuffer大小
	int shaderType;//shader类型
	std::map<std::string, DShaderPropertyDesc> properties;
};

/*meshbuffer描述*/
typedef struct DMeshBufferDesc
{
public:
	DMeshBufferDesc(){}
public:
	int vertexCount /*顶点数量*/, indexCount /*索引数量*/;
	float*vertices; /*顶点缓存*/
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
	DMeshRes(int vertexUsage /*顶点用法描述*/, bool dynamic /*是否为动态mesh*/);
	void Refresh(DMeshBufferDesc* desc); //更新顶点缓存
	void Refresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);//更新顶点缓存
	void DrawPrimitive(DMeshTopology topology);//绘制
	virtual void Release() = 0; //释放资源
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
	unsigned int GetPropertyCount() const;
	void Init(const char* content, char* vsfunc, char* psfunc);
	unsigned int GetResCount() const;
	void ApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	void Draw();
	bool IsInitialized();
	int GetVertexUsage();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const = 0;
	virtual bool HasProperty(const LPCSTR key) const = 0;
	virtual void Release() = 0;

protected:
	virtual bool OnInit(const char* content, char* vsfunc, char* psfunc) = 0;
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams) = 0;
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
	void GetResolution(float&, float&);

protected:

	float m_width;
	float m_height;
};