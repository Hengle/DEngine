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
			else if (strcmp(read, "Queue") == 0)
			{
				InterpretRenderQueue(ifile, subshader);
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
#ifdef _DGAPI_D3D9
				subshader->AddCompileTarget(DGRAPHICS_API_D3D9);
#endif
			}
			else if (strcmp(read, "d3d10") == 0)
			{
#ifdef _DGAPI_D3D10
				subshader->AddCompileTarget(DGRAPHICS_API_D3D10);
#endif
			}
			else if (strcmp(read, "d3d11") == 0)
			{
#ifdef _DGAPI_D3D11
				subshader->AddCompileTarget(DGRAPHICS_API_D3D11);
#endif
			}
		}
	}
}

void DShaderBlock::InterpretRenderQueue(ifstream &ifile, DSubShader * subShader)
{
	char read[64];
	ifile >> read;
	if (strcmp(read, "Opaque") == 0)
		subShader->SetRenderQueue(DRenderQueue_Opaque);
	else if (strcmp(read, "Transparent") == 0)
		subShader->SetRenderQueue(DRenderQueue_Transparent);
}

void DShaderBlock::InterpretPass(ifstream & ifile, DSubShader* subshader)
{
	bool isBegin = false;
	char read[128];
	DShaderPass* pass = NULL;
	if (subshader->IsSupport(DSystem::GetGraphicsMgr()->GetAPI())) {
		pass = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderPass();//new DShaderPass();
		subshader->AddPass(pass);
	}
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
			/*else if (strcmp(read, "Tags") == 0)
			{
				InterpretTags(ifile, pass);
			}*/
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

//void DShaderBlock::InterpretTags(ifstream & ifile, DShaderPass* pass)
//{
//	bool isBegin = false;
//	char read[128], funcname[32];
//	while (!ifile.eof())
//	{
//		ifile >> read;
//
//		if (!isBegin)
//		{
//			if (strcmp(read, "{") == 0)
//			{
//				isBegin = true;
//			}
//		}
//		else
//		{
//			if (strcmp(read, "}") == 0)
//			{
//				isBegin = false;
//				return;
//			}
//			else if (strcmp(read, "VertexFunc:") == 0)
//			{
//				ifile >> funcname;
//				if(pass != NULL)
//					pass->SetVertexFuncName(funcname);
//			}
//			else if (strcmp(read, "PixelFunc:") == 0)
//			{
//				ifile >> funcname;
//				if (pass != NULL)
//					pass->SetPixelFuncName(funcname);
//			}
//		}
//	}
//}

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
				if (pass != NULL)
					pass->SetZWrite(state);
			}
			else if (strcmp(read, "ztest") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetZTest(state);
			}
			else if (strcmp(read, "cull") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetCullMode(state);
			}
			else if (strcmp(read, "blendop") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetBlendOp(state);
			}
			else if (strcmp(read, "blend") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetBlendSrcFactor(state);
				ifile >> state;
				if (pass != NULL)
					pass->SetBlendDstFactor(state);
			}
			else if (strcmp(read, "blendop") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetBlendOp(state);
			}
			else if (strcmp(read, "fill") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetFillMode(state);
			}
			else if (strcmp(read, "stencil") == 0)
			{
				if (pass != NULL)
					pass->SetStencilEnable(true);
				InterpretStencil(ifile, pass);
			}
		}
	}
}

void DShaderBlock::InterpretStencil(ifstream & ifile, DShaderPass * pass)
{
	bool isBegin = false;
	char read[64], state[64];
	unsigned short value;
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
			else if (strcmp(read, "ref") == 0)
			{
				ifile >> value;
				if (pass != NULL)
					pass->SetStencilId(value);
			}
			else if (strcmp(read, "readmask") == 0)
			{
				ifile >> value;
				if (pass != NULL)
					pass->SetStencilReadMask(value);
			}
			else if (strcmp(read, "writemask") == 0)
			{
				ifile >> value;
				if (pass != NULL)
					pass->SetStencilWriteMask(value);
			}
			else if (strcmp(read, "comp") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetStencilComp(state);
			}
			else if (strcmp(read, "pass") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetStencilPass(state);
			}
			else if (strcmp(read, "fail") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetStencilFail(state);
			}
			else if (strcmp(read, "zfail") == 0)
			{
				ifile >> state;
				if (pass != NULL)
					pass->SetStencilZFail(state);
			}
		}
	}
}

void DShaderBlock::InterpretShader(ifstream & ifile, DShaderPass * pass)
{
	bool isBegin = false;
	//string s;
	char read = 0;
	while (!ifile.eof())
	{
		read = ifile.get();

		if (!isBegin)
		{
			if (read == '{')
			{
				isBegin = true;
			}
		}
		else
		{
			if (read == '}')
			{
				isBegin = false;

				//const char* content = s.c_str();

				//if (pass != NULL)
				//	pass->CompileShader(content);

				return;
			}
			else
			{
				//s.push_back(read);
				if (pass != NULL)
				{
					pass->CompileShader(ifile);
				}
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

DRenderQueue DShaderBlock::GetRenderQueue()
{
	return m_supportShader->GetRenderQueue();;
}

DSubShader::DSubShader()
{
	m_compileTarget = 0;
	m_renderQueue = DRenderQueue_Opaque;
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
	return (m_compileTarget & api) != 0;
}

void DSubShader::SetRenderQueue(DRenderQueue renderQueue)
{
	m_renderQueue = renderQueue;
}

int DSubShader::GetPassCount()
{
	return m_passes.size();
}

DRenderQueue DSubShader::GetRenderQueue()
{
	return m_renderQueue;
}

DShaderPass * DSubShader::GetPass(int index)
{
	if (index >= 0 && index < m_passes.size())
		return m_passes.at(index);
	return NULL;
}