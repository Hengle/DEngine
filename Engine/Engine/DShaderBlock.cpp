#include "DShaderBlock.h"
#include "DSystem.h"
#include <string>

DShaderBlock::DShaderBlock()
{
}

DShaderBlock::~DShaderBlock()
{
}

void DShaderBlock::Release()
{
	if (m_supportShader != NULL)
	{
		m_supportShader->Release();
		delete m_supportShader;
		m_supportShader = NULL;
	}
}

bool DShaderBlock::Compile(char * fileName)
{
	ifstream ifile;
	ifile.open(fileName);
	bool result = false;
	if (ifile.fail())
	{
		return false;
	}
	char read[64];
	while (!ifile.eof())
	{
		ifile >> read;
		if (strcmp(read, "SubShader") == 0)
		{
			result = InterpretSubShader(ifile);
			if (result)
				break;
		}
	}

	ifile.close();

	return true;
}

bool DShaderBlock::InterpretSubShader(ifstream & ifile)
{
	bool isBegin = false;
	char read[128];
	DSubShader* subshader = NULL;
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				subshader = new DSubShader();
				//m_subShaders.push_back(subshader);
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0) 
			{
				isBegin = false;
				if (subshader != NULL)
				{
					if (subshader->IsSupport(DSystem::GetGraphicsMgr()->GetAPI()))
					{
						m_supportShader = subshader;
						return true;
					}
					subshader->Release();
					delete subshader;
					subshader = NULL;
				}
				return false;
			}
			else if (strcmp(read, "Desc") == 0)
			{
				InterpretDesc(ifile, subshader);
			}
			else if (strcmp(read, "Pass") == 0)
			{
				InterpretPass(ifile, subshader);
			}
		}
	}
	return false;
}

void DShaderBlock::InterpretDesc(ifstream & ifile, DSubShader * subshader)
{
	bool isBegin = false;
	char read[128], funcname[32];
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0)
			{
				isBegin = false;
				return;
			}
			else if (strcmp(read, "CompileTarget:") == 0)
			{
				InterpretCompileTarget(ifile, subshader);
			}
		}
	}
}

void DShaderBlock::InterpretCompileTarget(ifstream & ifile, DSubShader* subshader)
{
	bool isBegin = false;
	char read[64];
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0)
			{
				isBegin = false;
				return;
			}
			else if (strcmp(read, "d3d9") == 0)
			{
				subshader->AddCompileTarget(DGRAPHICS_API_D3D9);
			}
			else if (strcmp(read, "d3d10") == 0)
			{
				subshader->AddCompileTarget(DGRAPHICS_API_D3D10);
			}
			else if (strcmp(read, "d3d11") == 0)
			{
				subshader->AddCompileTarget(DGRAPHICS_API_D3D11);
			}
		}
	}
}

void DShaderBlock::InterpretPass(ifstream & ifile, DSubShader* subshader)
{
	bool isBegin = false;
	char read[128];
	DShaderPass* pass = new DShaderPass();
	subshader->AddPass(pass);
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0)
			{
				isBegin = false;
				return;
			}
			else if (strcmp(read, "Tags") == 0)
			{
				InterpretTags(ifile, pass);
			}
			else if (strcmp(read, "State") == 0)
			{
				InterpretState(ifile, pass);
			}
			else if (strcmp(read, "Shader") == 0)
			{
				InterpretShader(ifile, pass);
			}
		}
	}
}

void DShaderBlock::InterpretTags(ifstream & ifile, DShaderPass* pass)
{
	bool isBegin = false;
	char read[128], funcname[32];
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0)
			{
				isBegin = false;
				return;
			}
			else if (strcmp(read, "VertexFunc:") == 0)
			{
				ifile >> funcname;
				pass->SetVertexFuncName(funcname);
			}
			else if (strcmp(read, "PixelFunc:") == 0)
			{
				ifile >> funcname;
				pass->SetPixelFuncName(funcname);
			}
		}
	}
}

void DShaderBlock::InterpretState(ifstream & ifile, DShaderPass* pass)
{
	bool isBegin = false;
	char read[64],state[64];
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0)
			{
				isBegin = false;
				return;
			}
			else if (strcmp(read, "zwrite") == 0)
			{
				ifile >> state;
				pass->SetZWrite(state);
			}
			else if (strcmp(read, "ztest") == 0)
			{
				ifile >> state;
				pass->SetZTest(state);
			}
			else if (strcmp(read, "cull") == 0)
			{
				ifile >> state;
				pass->SetCullMode(state);
			}
		}
	}
}

void DShaderBlock::InterpretShader(ifstream & ifile, DShaderPass * pass)
{
	bool isBegin = false;
	string s;
	char read = 0;
	while (!ifile.eof())
	{
		read = ifile.get();

		if (!isBegin)
		{
			if (read == '[')
			{
				isBegin = true;
			}
		}
		else
		{
			if (read == ']')
			{
				isBegin = false;

				const char* content = s.c_str();

				pass->CompileShader(content);

				return;
			}
			else
			{
				s.push_back(read);
			}
		}
	}
}

bool DShaderBlock::IsSupported()
{
	return m_supportShader != NULL;
}

int DShaderBlock::GetPassCount()
{
	return m_supportShader->GetPassCount();
}

DShaderPass * DShaderBlock::GetPass(int index)
{
	return m_supportShader->GetPass(index);
}

DSubShader::DSubShader()
{
	m_compileTarget = 0;
}

void DSubShader::Release()
{
	
	int i;
	for (i = 0; i < m_passes.size(); i++)
	{
		DShaderPass* pass = m_passes[i];
		pass->Release();
		delete pass;
	}
	m_passes.clear();
}

void DSubShader::AddCompileTarget(DGraphicsAPI api)
{
	m_compileTarget |= api;
}

void DSubShader::AddPass(DShaderPass * pass)
{
	m_passes.push_back(pass);
}

bool DSubShader::IsSupport(DGraphicsAPI api)
{
	return m_compileTarget & api != 0;
}

int DSubShader::GetPassCount()
{
	return m_passes.size();
}

DShaderPass * DSubShader::GetPass(int index)
{
	if (index >= 0 && index < m_passes.size())
		return m_passes.at(index);
	return NULL;
}

DShaderPass::DShaderPass()
{
	m_vertexFuncName = 0;
	m_pixelFuncName = 0;
	m_zwrite = true;
	m_ztest = DRSCompareFunc_LEqual;
	m_cullmode = DCullMode_Back;
}

void DShaderPass::Release()
{
	if(m_vertexFuncName != 0)
		delete[] m_vertexFuncName;
	if(m_pixelFuncName != 0)
		delete[] m_pixelFuncName;
	m_vertexFuncName = 0;
	m_pixelFuncName = 0;
	if (m_shaderRes != NULL)
	{
		m_shaderRes->Release();
		delete m_shaderRes;
		m_shaderRes = NULL;
	}
}

void DShaderPass::SetZWrite(char * state)
{
	if (strcmp(state, "on") == 0)
		m_zwrite = true;
	else if (strcmp(state, "off") == 0)
		m_zwrite = false;
}

void DShaderPass::SetZTest(char * state)
{
	if (strcmp(state, "lequal") == 0)
		m_ztest = DRSCompareFunc_LEqual;
	else if (strcmp(state, "never") == 0)
		m_ztest = DRSCompareFunc_Never;
	else if (strcmp(state, "less") == 0)
		m_ztest = DRSCompareFunc_Less;
	else if (strcmp(state, "equal") == 0)
		m_ztest = DRSCompareFunc_Equal;
	else if (strcmp(state, "greater") == 0)
		m_ztest = DRSCompareFunc_Greater;
	else if (strcmp(state, "notequal") == 0)
		m_ztest = DRSCompareFunc_NotEqual;
	else if (strcmp(state, "gequal") == 0)
		m_ztest = DRSCompareFunc_GEqual;
	else if (strcmp(state, "always") == 0)
		m_ztest = DRSCompareFunc_Always;
}

void DShaderPass::SetCullMode(char * state)
{
	if (strcmp(state, "off") == 0)
		m_cullmode = DCullMode_Off;
	else if (strcmp(state, "back") == 0)
		m_cullmode = DCullMode_Back;
	else if (strcmp(state, "front") == 0)
		m_cullmode = DCullMode_Front;
}

void DShaderPass::SetVertexFuncName(char *vertexFuncName)
{
	int len = strlen(vertexFuncName) + 1;
	m_vertexFuncName = new char[len];
	strcpy_s(m_vertexFuncName, len, vertexFuncName);
}

void DShaderPass::SetPixelFuncName(char *pixelFuncName)
{
	int len = strlen(pixelFuncName) + 1;
	m_pixelFuncName = new char[len];
	strcpy_s(m_pixelFuncName, len, pixelFuncName);
}

void DShaderPass::CompileShader(const char * content)
{
	DShaderRes* res = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderRes();
	if (res != NULL)
	{
		res->Init(content, m_vertexFuncName, m_pixelFuncName);
		m_shaderRes = res;
	}
}

DShaderRes * DShaderPass::GetShaderRes()
{
	return m_shaderRes;
}

void DShaderPass::ApplyStates()
{
	DGraphics::SetCullMode(m_cullmode);
	DGraphics::SetZTestFunc(m_ztest);
	DGraphics::SetZWriteEnable(m_zwrite);
}
