#pragma once
#include "DGraphicsDefine.h"
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

private:
	bool m_zwrite;
	DRSCompareFunc m_ztest;
	DCullMode m_cullmode;
};

class DSubShader
{
public:
	DSubShader();
	void Release();
	void SetVertexFuncName(char*);
	void SetPixelFuncName(char*);
	void AddCompileTarget(DGraphicsAPI api);
	void AddPass(DShaderPass*);

private:
	char* m_vertexFuncName;
	char* m_pixelFuncName;
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

private:
	void InterpretSubShader(ifstream&);
	void InterpretTags(ifstream&, DSubShader*);
	void InterpretCompileTarget(ifstream&, DSubShader*);
	void InterpretPass(ifstream&, DSubShader*);
	void InterpretState(ifstream&, DShaderPass*);
	void InterpretShader(ifstream&, DShaderPass*);

private:
	vector<DSubShader*> m_subShaders;
};

