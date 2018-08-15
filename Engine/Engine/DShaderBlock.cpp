#include "DShaderBlock.h"
#include "DSystem.h"
#include <string>

DShaderBlock::DShaderBlock()
{
	m_renderQueue = DRenderQueue_Opaque;
}

DShaderBlock::~DShaderBlock()
{
}

void DShaderBlock::Release()
{
	int i;
	for (i = 0; i < m_passes.size(); i++)
	{
		DShaderPassLink passLink = m_passes[i];
		//if (passLink != NULL)
		{
			if (passLink.pass != NULL)
			{
				passLink.pass->Release();
				delete passLink.pass;
			}
			//delete passLink;
		}
	}
	m_passes.clear();
	/*if (m_supportShader != NULL)
	{
		m_supportShader->Release();
		delete m_supportShader;
		m_supportShader = NULL;
	}*/
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
		if (strcmp(read, "ShaderBlock") == 0)
		{
			result = InterpretShaderBlock(ifile);
			//result = InterpretSubShader(ifile);
			if (result)
				break;
		}
	}

	ifile.close();

	return true;
}

//bool DShaderBlock::InterpretSubShader(ifstream & ifile)
//{
//	bool isBegin = false;
//	char read[128];
//	DSubShader* subshader = NULL;
//	while (!ifile.eof())
//	{
//		ifile >> read;
//
//		if (!isBegin)
//		{
//			if (strcmp(read, "{") == 0)
//			{
//				subshader = new DSubShader();
//				//m_subShaders.push_back(subshader);
//				isBegin = true;
//			}
//		}
//		else
//		{
//			if (strcmp(read, "}") == 0) 
//			{
//				isBegin = false;
//				if (subshader != NULL)
//				{
//					if (subshader->IsSupport(DSystem::GetGraphicsMgr()->GetAPI()))
//					{
//						m_supportShader = subshader;
//						return true;
//					}
//					subshader->Release();
//					delete subshader;
//					subshader = NULL;
//				}
//				return false;
//			}
//			else if (strcmp(read, "Desc") == 0)
//			{
//				InterpretDesc(ifile, subshader);
//			}
//			else if (strcmp(read, "Pass") == 0)
//			{
//				InterpretPass(ifile, subshader);
//			}
//		}
//	}
//	return false;
//}

bool DShaderBlock::InterpretShaderBlock(ifstream & ifile)
{
	bool isBegin = false;
	char read[128];
	//DSubShader* subshader = NULL;
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				//subshader = new DSubShader();
				//m_subShaders.push_back(subshader);
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0)
			{
				isBegin = false;
				/*if (subshader != NULL)
				{
					if (subshader->IsSupport(DSystem::GetGraphicsMgr()->GetAPI()))
					{
						m_supportShader = subshader;
						return true;
					}
					subshader->Release();
					delete subshader;
					subshader = NULL;
				}*/
				return true;
			}
			else if (strcmp(read, "Desc") == 0)
			{
				InterpretDesc(ifile);
			}
			else if (strcmp(read, "Pass") == 0)
			{
				InterpretPass(ifile);
			}
			else if (strcmp(read, "LinkPass") == 0)
			{

			}
		}
	}
	return false;
}

void DShaderBlock::InterpretDesc(ifstream & ifile)
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
			/*else if (strcmp(read, "CompileTarget:") == 0)
			{
				InterpretCompileTarget(ifile, subshader);
			}*/
			else if (strcmp(read, "Queue") == 0)
			{
				InterpretRenderQueue(ifile);
			}
		}
	}
}

//void DShaderBlock::InterpretCompileTarget(ifstream & ifile, DSubShader* subshader)
//{
//	bool isBegin = false;
//	char read[64];
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
//			else if (strcmp(read, "d3d9") == 0)
//			{
//#ifdef _DGAPI_D3D9
//				subshader->AddCompileTarget(DGRAPHICS_API_D3D9);
//#endif
//			}
//			else if (strcmp(read, "d3d10") == 0)
//			{
//#ifdef _DGAPI_D3D10
//				subshader->AddCompileTarget(DGRAPHICS_API_D3D10);
//#endif
//			}
//			else if (strcmp(read, "d3d11") == 0)
//			{
//#ifdef _DGAPI_D3D11
//				subshader->AddCompileTarget(DGRAPHICS_API_D3D11);
//#endif
//			}
//			else if (strcmp(read, "opengl") == 0)
//			{
//#ifdef _DGAPI_OPENGL
//				subshader->AddCompileTarget(DGRAPHICS_API_OPENGL);
//#endif
//			}
//		}
//	}
//}

void DShaderBlock::InterpretRenderQueue(ifstream &ifile)
{
	char read[64];
	ifile >> read;
	if (strcmp(read, "Opaque") == 0)
		//subShader->SetRenderQueue(DRenderQueue_Opaque);
		m_renderQueue = DRenderQueue_Opaque;
	else if (strcmp(read, "Transparent") == 0)
		//subShader->SetRenderQueue(DRenderQueue_Transparent);
		m_renderQueue = DRenderQueue_Transparent;
}

void DShaderBlock::InterpretPass(ifstream & ifile)
{
	bool isBegin = false;
	char read[128];
	DShaderPass* pass = NULL;
	DGraphicsAPI api = DSystem::GetGraphicsMgr()->GetAPI();
	//if (subshader->IsSupport(DSystem::GetGraphicsMgr()->GetAPI())) {
		pass = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderPass();//new DShaderPass();
	//	subshader->AddPass(pass);
	//}
		bool isSupport = false;
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
				if (isSupport)
				{
					DShaderPassLink link;
					link.pass = pass;
					m_passes.push_back(link);
				}
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
			else if (strcmp(read, "SHADER_BEGIN:") == 0)
			{
				int targets = InterpretCompileTarget(ifile); //先解析该shader块的支持api
				if ((targets & api) != 0 && !isSupport) //如果该shader块支持则解析并编译该shader代码
				{
					//InterpretShader(ifile, pass);
					if (pass != NULL)
					{
						pass->CompileShader(ifile);
					}
					isSupport = true;
				}
				//else
				//{
					while (!ifile.eof()) //如果该shader不支持，则将剩余部分读完并跳过
					{
						ifile >> read;
						if (strcmp(read, "SHADER_END") == 0)
							break;
					}
				//}
				
			}
		}
	}
}

void DShaderBlock::InterpretLinkPass(ifstream & ifile)
{
	bool isBegin = false;
	char read[128];
	unsigned int groupid, resid, passid;

	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "{") == 0)
			{
				ifile >> groupid >> resid >> passid;
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "}") == 0)
			{
				isBegin = false;
				DShaderPassLink link;
				link.pass = 0;
				link.groupid = groupid;
				link.resid = resid;
				link.passid = passid;
				//if (isSupport)
					m_passes.push_back(link);
				return;
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

//void DShaderBlock::InterpretShader(ifstream & ifile, DShaderPass * pass)
//{
//	//bool isBegin = false;
//	//string s;
//	char read[128];
//	//int compileTarget = 0;
//	//DGraphicsAPI api = DSystem::GetGraphicsMgr()->GetAPI();
//	while (!ifile.eof())
//	{
//		ifile >> read;
//
//		if (strcmp(read, "SHADER_END") == 0)
//		{
//			return;
//		}
//		if (pass != NULL)
//		{
//			pass->CompileShader(ifile);
//		}
//
//		/*if (!isBegin)
//		{
//			if (strcmp(read, "{") == 0)
//			{
//				isBegin = true;
//
//				if (pass != NULL)
//				{
//					pass->CompileShader(ifile);
//				}
//				else
//				{
//					ifile >> read;
//					while (strcmp(read, "]") != 0)
//					{
//						ifile >> read;
//					}
//				}
//			}
//		}
//		else
//		{
//			if (strcmp(read, "}") == 0)
//			{
//				isBegin = false;
//
//				return;
//			}
//		}*/
//	}
//}

int DShaderBlock::InterpretCompileTarget(ifstream & ifile)
{
	bool isBegin = false;
	char read[64];
	int compileTarget = 0;
	while (!ifile.eof())
	{
		ifile >> read;

		if (!isBegin)
		{
			if (strcmp(read, "[") == 0)
			{
				isBegin = true;
			}
		}
		else
		{
			if (strcmp(read, "]") == 0)
			{
				isBegin = false;
				return compileTarget;
			}
			else if (strcmp(read, "d3d9") == 0)
			{
#ifdef _DGAPI_D3D9
				compileTarget |= DGRAPHICS_API_D3D9;
#endif
			}
			else if (strcmp(read, "d3d10") == 0)
			{
#ifdef _DGAPI_D3D10
				compileTarget |= DGRAPHICS_API_D3D10;
#endif
			}
			else if (strcmp(read, "d3d11") == 0)
			{
#ifdef _DGAPI_D3D11
				compileTarget |= DGRAPHICS_API_D3D11;
#endif
			}
			else if (strcmp(read, "opengl") == 0)
			{
#ifdef _DGAPI_OPENGL
				compileTarget |= DGRAPHICS_API_OPENGL;
#endif
			}
		}
	}
	return compileTarget;
}

bool DShaderBlock::IsSupported()
{
	return m_passes.size() > 0;
	//return m_supportShader != NULL;
}

int DShaderBlock::GetPassCount()
{
	return m_passes.size();
	//return m_supportShader->GetPassCount();
}

DShaderPass * DShaderBlock::GetPass(int index)
{
	if (index < 0 || index >= m_passes.size())
		return NULL;
	DShaderPassLink link = m_passes.at(index);
	if (link.pass != NULL)
		return link.pass;
	else
	{
		DShader* shader = DRes::Load<DShader>(link.groupid, link.resid);
		if (shader != NULL)
			return shader->GetPass(link.passid);
	}
	return NULL;
	//return m_supportShader->GetPass(index);
}

DRenderQueue DShaderBlock::GetRenderQueue()
{
	return m_renderQueue;
}

//bool DShaderBlock::HasProperty(const LPCSTR key, int pass)
//{
//	if (pass < 0 || pass >= m_passes.size())
//		return false;
//	DShaderPassLink link = m_passes.at(pass);
//	if (link.pass != NULL)
//		return link.pass->HasProperty(key);
//	else
//	{
//		DShader* shader = DRes::Load<DShader>(link.groupid, link.resid);
//		if (shader != NULL)
//		{
//			return shader->HasProperty(key, pass);
//		}
//	}
//	return false;
//}
//
//unsigned int DShaderBlock::GetShaderProgramCount(int pass)
//{
//	if (pass < 0 || pass >= m_passes.size())
//		return false;
//	DShaderPassLink link = m_passes.at(pass);
//	if (link.pass != NULL)
//		return link.pass->GetShaderProgramCount();
//	else
//	{
//		DShader* shader = DRes::Load<DShader>(link.groupid, link.resid);
//		if (shader != NULL)
//		{
//			return shader->HasProperty(key, pass);
//		}
//	}
//	return 0;
//}
//
//void DShaderBlock::Draw(int pass)
//{
//}
//
//DShaderProgram * DShaderBlock::GetShaderProgram(int pass, unsigned int)
//{
//	return nullptr;
//}

//DSubShader::DSubShader()
//{
//	m_compileTarget = 0;
//	m_renderQueue = DRenderQueue_Opaque;
//}
//
//void DSubShader::Release()
//{
//	
//	int i;
//	for (i = 0; i < m_passes.size(); i++)
//	{
//		DShaderPass* pass = m_passes[i];
//		pass->Release();
//		delete pass;
//	}
//	m_passes.clear();
//}
//
//void DSubShader::AddCompileTarget(DGraphicsAPI api)
//{
//	m_compileTarget |= api;
//}
//
//void DSubShader::AddPass(DShaderPass * pass)
//{
//	m_passes.push_back(pass);
//}
//
//bool DSubShader::IsSupport(DGraphicsAPI api)
//{
//	return (m_compileTarget & api) != 0;
//}
//
//void DSubShader::SetRenderQueue(DRenderQueue renderQueue)
//{
//	m_renderQueue = renderQueue;
//}
//
//int DSubShader::GetPassCount()
//{
//	return m_passes.size();
//}
//
//DRenderQueue DSubShader::GetRenderQueue()
//{
//	return m_renderQueue;
//}
//
//DShaderPass * DSubShader::GetPass(int index)
//{
//	if (index >= 0 && index < m_passes.size())
//		return m_passes.at(index);
//	return NULL;
//}