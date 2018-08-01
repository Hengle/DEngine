#pragma once
#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11

#include "DGLCore.h"
#include "DGraphicsDefine.h"
#include "DShaderPass.h"

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

//class DGeometryResD3D : public DGeometryRes
//{
//public:
//	DGeometryResD3D(int vertexUsage, bool dynamic);
//
//protected:
//	virtual void OnRefresh(DGeometryBufferDesc* desc);
//	virtual bool OnInit(DGeometryBufferDesc* desc);
//	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
//	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount) = 0;
//};

class D3DShaderProgram : public DShaderProgram
{
public:
	/*初始化*/
	void Init(const char* content /*shader内容*/, char* funcName /*函数名*/);

protected:
	virtual bool OnInit(const char* content, char* funcName) = 0;
};

class D3DShaderPass : public DShaderPass
{
public:
	D3DShaderPass();
	virtual void Release();
	virtual void Draw();
	virtual int GetVertexUsage();
	virtual bool HasProperty(LPCSTR key);
	virtual void CompileShader(std::ifstream&);
	virtual int GetShaderProgramCount();
	virtual DShaderProgram* GetShaderProgram(int index);

protected:
	virtual void OnCompile(const char*) = 0;

private:
	void CompileShaderContent(std::ifstream&);
	void SetVertexFuncName(char*);
	void SetPixelFuncName(char*);

protected:
	DShaderProgram* m_vertexShader;
	DShaderProgram* m_pixelShader;
	char* m_vertexFuncName;
	char* m_pixelFuncName;
};

#endif