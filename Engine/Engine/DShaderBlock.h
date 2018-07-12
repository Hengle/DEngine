#pragma once
#include "DGraphicsDefine.h"
#include "DGLCore.h"
#include <fstream>
#include <vector>

using namespace std;

class DShaderPass
{
public:
	DShaderPass();
	void Release();
	void SetZWrite(char*);
	void SetZTest(char*);
	void SetCullMode(char*);
	void SetFillMode(char*);
	void SetBlendOp(char*);
	void SetBlendSrcFactor(char*);
	void SetBlendDstFactor(char*);
	void SetStencilEnable(bool);
	void SetStencilId(unsigned short);
	void SetStencilReadMask(unsigned short);
	void SetStencilWriteMask(unsigned short);
	void SetStencilComp(char*);
	void SetStencilPass(char*);
	void SetStencilFail(char*);
	void SetStencilZFail(char*);
	void SetVertexFuncName(char*);
	void SetPixelFuncName(char*);
	void CompileShader(const char*);
	//DShaderRes* GetShaderRes();
	DShaderProgram* GetVertexShader();
	DShaderProgram* GetPixelShader();
	void ApplyStates();

	void SetPassEnable(bool);
	bool IsPassEnable();

private:
	DRSBlendFactor GetBlendFactor(char*);
	DRSCompareFunc GetCompFunc(char*);
	DRSStencilOp GetStencilOp(char*);

private:
	bool m_zwrite;
	bool m_enableStencil;
	DRSCompareFunc m_ztest;
	DCullMode m_cullmode;
	DFillMode m_fillmode;
	//DShaderRes* m_shaderRes;
	DShaderProgram* m_vertexShader;
	DShaderProgram* m_pixelShader;
	DRSBlendOp m_blendOp;
	DRSBlendFactor m_blendSrc;
	DRSBlendFactor m_blendDst;
	unsigned short m_stencilId;
	unsigned short m_stencilReadMask;
	unsigned short m_stencilWriteMask;
	DRSCompareFunc m_stencilComp;
	DRSStencilOp m_stencilPass;
	DRSStencilOp m_stencilFail;
	DRSStencilOp m_stencilZFail;
	bool m_enableBlend;
	char* m_vertexFuncName;
	char* m_pixelFuncName;
	
	bool m_passEnable;
};

class DSubShader
{
public:
	DSubShader();
	void Release();
	void AddCompileTarget(DGraphicsAPI api);
	void AddPass(DShaderPass*);
	bool IsSupport(DGraphicsAPI api);
	int GetPassCount();
	DShaderPass* GetPass(int);

private:
	int m_compileTarget;
	vector<DShaderPass*> m_passes;
};

/*shader代码块*/
class DShaderBlock
{
public:
	DShaderBlock();
	~DShaderBlock();
	void Release();

	/*编译*/
	bool Compile(char* fileName);
	bool IsSupported();
	int GetPassCount();
	DShaderPass* GetPass(int);

private:
	/*解释sub shader块*/
	bool InterpretSubShader(ifstream&);
	/*解释desc块*/
	void InterpretDesc(ifstream&, DSubShader*);
	/*解释编译目标块*/
	void InterpretCompileTarget(ifstream&, DSubShader*);
	/*解释pass块*/
	void InterpretPass(ifstream&, DSubShader*);
	/*解释标签块*/
	void InterpretTags(ifstream&, DShaderPass*);
	/*解释状态块*/
	void InterpretState(ifstream&, DShaderPass*);
	/*解释模板状态块*/
	void InterpretStencil(ifstream&, DShaderPass*);
	/*解释shader块*/
	void InterpretShader(ifstream&, DShaderPass*);

private:
	DSubShader* m_supportShader;
};

