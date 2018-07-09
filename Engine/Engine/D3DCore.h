#pragma once
#include "DGLCore.h"
#include "DGraphicsDefine.h"

struct RasterizerState
{
public:
	unsigned int GetKey();

public:
	DCullMode cullMode;
	DFillMode fillMode;
};

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
