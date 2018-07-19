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
		pass = new DShaderPass();
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
				if(pass != NULL)
					pass->SetVertexFuncName(funcname);
			}
			else if (strcmp(read, "PixelFunc:") == 0)
			{
				ifile >> funcname;
				if (pass != NULL)
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

				if (pass != NULL)
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

DShaderPass::DShaderPass()
{
	m_vertexFuncName = 0;
	m_pixelFuncName = 0;
	m_zwrite = true;
	m_ztest = DRSCompareFunc_LEqual;
	m_cullmode = DCullMode_Back;
	m_fillmode = DFillMode_Solid;
	m_enableBlend = false;
	m_blendOp = DRSBlendOp_Add;
	m_blendSrc = DRSBlendFactor_SrcAlpha;
	m_blendDst = DRSBlendFactor_OneMinusSrcAlpha;
	m_enableStencil = false;
	m_stencilId = 0;
	m_stencilReadMask = 255;
	m_stencilWriteMask = 255;
	m_stencilComp = DRSCompareFunc_Always;
	m_stencilPass = DRSStencilOp_Keep;
	m_stencilFail = DRSStencilOp_Keep;
	m_stencilZFail = DRSStencilOp_Keep;
	m_passEnable = true;
}

void DShaderPass::Release()
{
	if(m_vertexFuncName != 0)
		delete[] m_vertexFuncName;
	if(m_pixelFuncName != 0)
		delete[] m_pixelFuncName;
	m_vertexFuncName = 0;
	m_pixelFuncName = 0;
	/*if (m_shaderRes != NULL)
	{
		m_shaderRes->Release();
		delete m_shaderRes;
		m_shaderRes = NULL;
	}*/
	if (m_vertexShader != NULL)
	{
		m_vertexShader->Release();
		delete m_vertexShader;
		m_vertexShader = NULL;
	}
	if (m_pixelShader != NULL)
	{
		m_pixelShader->Release();
		delete m_pixelShader;
		m_pixelShader = NULL;
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
	m_ztest = GetCompFunc(state);
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

void DShaderPass::SetFillMode(char * state)
{
	if (strcmp(state, "solid") == 0)
		m_fillmode = DFillMode_Solid;
	else if (strcmp(state, "wireframe") == 0)
		m_fillmode = DFillMode_WireFrame;
}

void DShaderPass::SetBlendOp(char * state)
{
	if (strcmp(state, "add") == 0)
		m_blendOp = DRSBlendOp_Add;
	else if (strcmp(state, "sub") == 0)
		m_blendOp = DRSBlendOp_Sub;
	else if (strcmp(state, "revsub") == 0)
		m_blendOp = DRSBlendOp_RevSub;
	else if (strcmp(state, "min") == 0)
		m_blendOp = DRSBlendOp_Min;
	else if (strcmp(state, "max") == 0)
		m_blendOp = DRSBlendOp_Max;
}

void DShaderPass::SetBlendSrcFactor(char * state)
{
	m_blendSrc = GetBlendFactor(state);
	m_enableBlend = true;
}

void DShaderPass::SetBlendDstFactor(char * state)
{
	m_blendDst = GetBlendFactor(state);
	m_enableBlend = true;
}

void DShaderPass::SetStencilEnable(bool enable)
{
	m_enableStencil = enable;
}

void DShaderPass::SetStencilId(unsigned short stencilId)
{
	m_stencilId = stencilId;
}

void DShaderPass::SetStencilReadMask(unsigned short readMask)
{
	m_stencilReadMask = readMask;
}

void DShaderPass::SetStencilWriteMask(unsigned short writeMask)
{
	m_stencilWriteMask = writeMask;
}

void DShaderPass::SetStencilComp(char * state)
{
	m_stencilComp = GetCompFunc(state);
}

void DShaderPass::SetStencilPass(char * state)
{
	m_stencilPass = GetStencilOp(state);
}

void DShaderPass::SetStencilFail(char * state)
{
	m_stencilFail = GetStencilOp(state);
}

void DShaderPass::SetStencilZFail(char * state)
{
	m_stencilZFail = GetStencilOp(state);
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
	/*DShaderRes* res = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderRes();
	if (res != NULL)
	{
		res->Init(content, m_vertexFuncName, m_pixelFuncName);
		m_shaderRes = res;
	}*/
	DShaderProgram* vshader = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderProgram(DShaderProgram_Vertex);
	DShaderProgram* pshader = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderProgram(DShaderProgram_Pixel);
	if (vshader != NULL)
	{
		vshader->Init(content, m_vertexFuncName);
		m_vertexShader = vshader;
	}
	if (pshader != NULL)
	{
		pshader->Init(content, m_pixelFuncName);
		m_pixelShader = pshader;
	}
}

DShaderProgram * DShaderPass::GetVertexShader()
{
	return m_vertexShader;
}

DShaderProgram * DShaderPass::GetPixelShader()
{
	return m_pixelShader;
}

//DShaderRes * DShaderPass::GetShaderRes()
//{
//	return m_shaderRes;
//}

void DShaderPass::ApplyStates()
{
	DGraphics::SetCullMode(m_cullmode);
	DGraphics::SetFillMode(m_fillmode);
	DGraphics::SetZTestFunc(m_ztest);
	DGraphics::SetZWriteEnable(m_zwrite);
	DGraphics::SetBlendEnable(m_enableBlend);
	DGraphics::SetBlendOp(m_blendOp);
	DGraphics::SetBlendDstFactor(m_blendDst);
	DGraphics::SetBlendSrcFactor(m_blendSrc);
}

void DShaderPass::SetPassEnable(bool enable)
{
	m_passEnable = enable;
}

bool DShaderPass::IsPassEnable()
{
	return m_passEnable;
}

DRSBlendFactor DShaderPass::GetBlendFactor(char * state)
{
	if (strcmp(state, "one") == 0)
		return DRSBlendFactor_One;
	else if (strcmp(state, "zero") == 0)
		return DRSBlendFactor_Zero;
	else if (strcmp(state, "srccolor") == 0)
		return DRSBlendFactor_SrcColor;
	else if (strcmp(state, "srcalpha") == 0)
		return DRSBlendFactor_SrcAlpha;
	else if (strcmp(state, "dstcolor") == 0)
		return DRSBlendFactor_DstColor;
	else if (strcmp(state, "dstalpha") == 0)
		return DRSBlendFactor_DstAlpha;
	else if (strcmp(state, "oneminussrccolor") == 0)
		return DRSBlendFactor_OneMinusSrcColor;
	else if (strcmp(state, "oneminussrcalpha") == 0)
		return DRSBlendFactor_OneMinusSrcAlpha;
	else if (strcmp(state, "oneminusdstcolor") == 0)
		return DRSBlendFactor_OneMinusDstColor;
	else if (strcmp(state, "oneminusdstalpha") == 0)
		return DRSBlendFactor_OneMinusDstAlpha;
	return DRSBlendFactor_Zero;
}

DRSCompareFunc DShaderPass::GetCompFunc(char * state)
{
	if (strcmp(state, "lequal") == 0)
		return DRSCompareFunc_LEqual;
	else if (strcmp(state, "never") == 0)
		return DRSCompareFunc_Never;
	else if (strcmp(state, "less") == 0)
		return DRSCompareFunc_Less;
	else if (strcmp(state, "equal") == 0)
		return DRSCompareFunc_Equal;
	else if (strcmp(state, "greater") == 0)
		return DRSCompareFunc_Greater;
	else if (strcmp(state, "notequal") == 0)
		return DRSCompareFunc_NotEqual;
	else if (strcmp(state, "gequal") == 0)
		return DRSCompareFunc_GEqual;
	else if (strcmp(state, "always") == 0)
		return DRSCompareFunc_Always;
	else
		return DRSCompareFunc_Always;
}

DRSStencilOp DShaderPass::GetStencilOp(char * state)
{
	if (strcmp(state, "keep") == 0)
		return DRSStencilOp_Keep;
	else if (strcmp(state, "zero") == 0)
		return DRSStencilOp_Zero;
	else if (strcmp(state, "replace") == 0)
		return DRSStencilOp_Replace;
	else if (strcmp(state, "incrsat") == 0)
		return DRSStencilOp_IncrementSaturate;
	else if (strcmp(state, "decrsat") == 0)
		return DRSStencilOp_DecrementSaturate;
	else if (strcmp(state, "invert") == 0)
		return DRSStencilOp_Invert;
	else if (strcmp(state, "incrwrap") == 0)
		return DRSStencilOp_IncrementWrap;
	else if (strcmp(state, "decrwrap") == 0)
		return DRSStencilOp_DecrementWrap;
	else
		return DRSStencilOp_Keep;
}
