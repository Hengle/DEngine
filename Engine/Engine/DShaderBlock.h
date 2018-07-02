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
	void SetVertexFuncName(char*);
	void SetPixelFuncName(char*);
	void CompileShader(const char*);
	DShaderRes* GetShaderRes();
	void ApplyStates();

private:
	bool m_zwrite;
	DRSCompareFunc m_ztest;
	DCullMode m_cullmode;
	DShaderRes* m_shaderRes;
	char* m_vertexFuncName;
	char* m_pixelFuncName;
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

class DShaderBlock
{
public:
	DShaderBlock();
	~DShaderBlock();
	void Release();

	bool Compile(char* fileName);
	bool IsSupported();
	int GetPassCount();
	DShaderPass* GetPass(int);

private:
	bool InterpretSubShader(ifstream&);
	void InterpretDesc(ifstream&, DSubShader*);
	void InterpretCompileTarget(ifstream&, DSubShader*);
	void InterpretPass(ifstream&, DSubShader*);
	void InterpretTags(ifstream&, DShaderPass*);
	void InterpretState(ifstream&, DShaderPass*);
	void InterpretShader(ifstream&, DShaderPass*);

private:
	//vector<DSubShader*> m_subShaders;
	DSubShader* m_supportShader;
};

