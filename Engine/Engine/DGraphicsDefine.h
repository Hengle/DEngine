#pragma once

enum DGraphicsAPI
{
	DGRAPHICS_API_D3D9 = 1,
	DGRAPHICS_API_D3D10 = 2,
	DGRAPHICS_API_D3D11 = 4,
};

enum DVertexUsage
{
	DVertexUsage_POSITION = 0,
	DVertexUsage_TEXCOORD0 = 1,
	DVertexUsage_TEXCOORD1 = 2,
	DVertexUsage_TEXCOORD2 = 3,
	DVertexUsage_TEXCOORD3 = 4,
	DVertexUsage_COLOR = 5,
	DVertexUsage_NORMAL = 6,
	DVertexUsage_TANGENT = 7,
	DVertexUsage_BINORMAL = 8,
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

enum DRSBlendFactor
{
	DRSBlendFactor_One              = 0,
	DRSBlendFactor_Zero             = 1,
	DRSBlendFactor_SrcColor         = 2,
	DRSBlendFactor_SrcAlpha         = 3,
	DRSBlendFactor_DstColor         = 4,
	DRSBlendFactor_DstAlpha         = 5,
	DRSBlendFactor_OneMinusSrcColor = 6,
	DRSBlendFactor_OneMinusSrcAlpha = 7,
	DRSBlendFactor_OneMinusDstColor = 8,
	DRSBlendFactor_OneMinusDstAlpha = 9,
};

enum DRSBlendOp
{
	DRSBlendOp_Add    = 0,
	DRSBlendOp_Sub    = 1,
	DRSBlendOp_RevSub = 2,
	DRSBlendOp_Min    = 3,
	DRSBlendOp_Max    = 4,
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

enum DVertexBufferType
{
	DVERTEX_BUFFER_TYPE_POSITION = 1,
	DVERTEX_BUFFER_TYPE_NORMAL = 2,
	DVERTEX_BUFFER_TYPE_COLOR = 4,
	DVERTEX_BUFFER_TYPE_UV0 = 8,
	DVERTEX_BUFFER_TYPE_UV1 = 16,
	DVERTEX_BUFFER_TYPE_UV2 = 32,
};