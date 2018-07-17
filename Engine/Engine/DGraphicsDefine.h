#pragma once

/*图形API定义*/
enum DGraphicsAPI
{
	DGRAPHICS_API_D3D9   = 1,
	DGRAPHICS_API_D3D10  = 2,
	DGRAPHICS_API_D3D11  = 4,
	DGRAPHICS_API_OPENGL = 8,
};

/*顶点用法定义*/
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

/*平铺模式定义*/
enum DWrapMode
{
	DWrapMode_Repeat = 0,
	DWrapMode_Clamp = 1,
};

/*几何体拓扑模式定义*/
enum DGeometryTopology
{
	DGeometryTopology_PointList,
	DGeometryTopology_LineStrip,
	DGeometryTopology_LineList,
	DGeometryTopology_TriangleStrip,
	DGeometryTopology_TriangleList,
};

/*背面裁剪模式定义*/
enum DCullMode
{
	DCullMode_Back = 0,
	DCullMode_Front = 1,
	DCullMode_Off = 2,
};

/*填充模式定义*/
enum DFillMode
{
	DFillMode_WireFrame = 0,
	DFillMode_Solid = 1,
};

/*渲染状态比较函数*/
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

/*渲染状态混合因子*/
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

/*渲染状态混合方式*/
enum DRSBlendOp
{
	DRSBlendOp_Add    = 0,
	DRSBlendOp_Sub    = 1,
	DRSBlendOp_RevSub = 2,
	DRSBlendOp_Min    = 3,
	DRSBlendOp_Max    = 4,
};

/*模板测试结果处理方式*/
enum DRSStencilOp
{
	DRSStencilOp_Keep = 0,
	DRSStencilOp_Zero = 1,
	DRSStencilOp_Replace = 2,
	DRSStencilOp_IncrementSaturate = 3,
	DRSStencilOp_DecrementSaturate = 4,
	DRSStencilOp_Invert = 5,
	DRSStencilOp_IncrementWrap = 6,
	DRSStencilOp_DecrementWrap = 7,
};

/*shader程序类型*/
enum DShaderProgramType
{
	DShaderProgram_Vertex = 0,
	DShaderProgram_Pixel = 1,
};