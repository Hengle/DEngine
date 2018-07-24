#pragma once
#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11

#include "DGLCore.h"
#include "DGraphicsDefine.h"

/*光栅化状态集*/
struct RasterizerState
{
public:
	unsigned int GetKey();

public:
	DCullMode cullMode;
	DFillMode fillMode;
};

/*深度模板状态集*/
struct DepthStencilState
{
public:
	unsigned long GetKey();

public:
	bool zwrite;
	bool enableStencil;
	UINT stencilId;
	DRSCompareFunc ztest;
	DRSCompareFunc stencilComp;
	DRSStencilOp stencilPassOp;
	DRSStencilOp stencilFailOp;
	DRSStencilOp stencilZFailOp;
	UINT8 stencilReadMask;
	UINT8 stencilWriteMask;
};

/*混合状态集*/
struct BlendState
{
public:
	unsigned long GetKey();

public:
	bool enableBlend;
	DRSBlendOp blendOp;
	DRSBlendFactor srcfactor;
	DRSBlendFactor dstfactor;
};

class DGeometryResD3D : public DGeometryRes
{
public:
	DGeometryResD3D(int vertexUsage, bool dynamic);

protected:
	virtual void OnRefresh(DGeometryBufferDesc* desc);
	virtual bool OnInit(DGeometryBufferDesc* desc);
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
};

#endif