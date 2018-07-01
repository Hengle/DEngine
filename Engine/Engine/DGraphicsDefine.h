﻿#pragma once

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