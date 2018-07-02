#include "DShader.h"
#include "DLog.h"
#include "DSystem.h"

void DShaderConstantTable::Release()
{
	std::map<std::string, float*>::iterator piter;
	for (piter = params.begin(); piter != params.end(); piter++)
	{
		if (piter->second != NULL)
		{
			delete[] piter->second;
		}
	}
	params.clear();

	std::map<std::string, DTexture*>::iterator  titer;
	for (titer = textures.begin(); titer != textures.end(); titer++)
	{
		if (titer->second != NULL)
		{
			titer->second->Destroy();
			delete titer->second;
		}
	}

	textures.clear();
}

void DShaderConstantTable::SetMatrix(const LPCSTR key, DMatrix4x4 & matrix)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[16];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 16; i++)
	{
		values[i] = matrix[i];
	}
}

void DShaderConstantTable::SetVector4(const LPCSTR key, DVector4 & vector)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[4];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 4; i++)
	{
		values[i] = vector[i];
	}
}

void DShaderConstantTable::SetVector3(const LPCSTR key, DVector3 & vector)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[3];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 3; i++)
	{
		values[i] = vector[i];
	}
}

void DShaderConstantTable::SetVector2(const LPCSTR key, DVector2 & vector)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[2];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 2; i++)
	{
		values[i] = vector[i];
	}
}

void DShaderConstantTable::SetColor(const LPCSTR key, DColor & color)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[4];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 4; i++)
	{
		values[i] = color[i];
	}
}

void DShaderConstantTable::SetFloat(const LPCSTR key, float value)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[1];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	values[0] = value;
}

void DShaderConstantTable::SetTexture(const LPCSTR key, DTexture * texture)
{
	if (textures.find(key) == textures.end())
	{
		textures.insert(std::pair<std::string, DTexture*>(key, texture));
	}
	else
	{
		textures[key] = texture;
	}
}

DShader::DShader()
{
	m_shaderBlock = 0;
	//m_shaderRes = 0;
}

DShader::~DShader()
{
}

//DShader * DShader::Create(WCHAR * vertexShader, WCHAR * pixelShader)
//{
//	
//	DShaderRes* sbf = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderRes();
//	if (sbf != NULL)
//	{
//		sbf->Init(vertexShader, pixelShader);
//		DShader* shader = new DShader();
//		shader->m_shaderRes = sbf;
//		return shader;
//	}
//	return NULL;
//}

DShader * DShader::Create(char * path)
{
	DShaderBlock* block = new DShaderBlock();
	block->Compile(path);
	if (!block->IsSupported())
	{
		block->Release();
		delete block;
		return NULL;
	}
	DShader* shader = new DShader();
	shader->m_shaderBlock = block;
	return shader;
}

void DShader::ReleaseGlobalConstants()
{
	if (sGlobalShaderConstants != nullptr)
	{
		sGlobalShaderConstants->Release();
		delete sGlobalShaderConstants;
		sGlobalShaderConstants = nullptr;
	}
}

void DShader::SetGlobalMatrix(const LPCSTR key, DMatrix4x4 & matrix)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetMatrix(key, matrix);
}

void DShader::SetGlobalVector4(const LPCSTR key, DVector4 & vector)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetVector4(key, vector);
}

void DShader::SetGlobalVector3(const LPCSTR key, DVector3 & vector)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetVector3(key, vector);
}

void DShader::SetGlobalVector2(const LPCSTR key, DVector2 & vector)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetVector2(key, vector);
}

void DShader::SetGlobalColor(const LPCSTR key, DColor & color)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetColor(key, color);
}

void DShader::SetGlobalFloat(const LPCSTR key, float value)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetFloat(key, value);
}

void DShader::SetGlobalTexture(const LPCSTR key, DTexture * texture)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetTexture(key, texture);
}

void DShader::Destroy()
{
	if (m_shaderBlock != NULL)
	{
		m_shaderBlock->Release();
		delete m_shaderBlock;
		m_shaderBlock = NULL;
	}
	/*if (m_shaderRes != NULL)
	{
		m_shaderRes->Release();
		delete m_shaderRes;
		m_shaderRes = NULL;
	}*/
}

//unsigned int DShader::GetCBufferCount() const
//{
//	if (m_shaderRes != NULL)
//		return m_shaderRes->GetCBufferCount();
//	return 0;
//}
//
//unsigned int DShader::GetPropertyCount() const
//{
//	if (m_shaderRes != NULL)
//		return m_shaderRes->GetPropertyCount();
//	return 0;
//}
//
//void DShader::GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const
//{
//	if (m_shaderRes != NULL)
//		m_shaderRes->GetPropertyInfo(key, desc);
//}

bool DShader::HasProperty(const LPCSTR key) const
{
	if (m_shaderBlock != NULL)
	{
		int scount,i;
		scount = m_shaderBlock->GetPassCount();
		DShaderPass* pass;
		DShaderRes* res;
		for (i = 0; i < scount; i++)
		{
			pass = m_shaderBlock->GetPass(i);
			if (pass == NULL)
				continue;
			res = pass->GetShaderRes();
			if (res == NULL)
				continue;
			if (res->HasProperty(key))
				return true;
		}
	}
	return false;
	//if (m_shaderRes != NULL)
	//	return m_shaderRes->HasProperty(key);
}

void DShader::ApplyParams(DShaderConstantTable * constantTable, int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return;
		DShaderRes* res = pass->GetShaderRes();
		if (res != NULL && constantTable != nullptr) {
			if (sGlobalShaderConstants == nullptr)
				sGlobalShaderConstants = new DShaderConstantTable();
			res->ApplyParams(constantTable->params, sGlobalShaderConstants->params);

			unsigned int rescount = res->GetResCount();
			if (rescount > 0)
			{
				int i;
				DShaderResDesc rdesc;
				DTexture* tex;
				for (i = 0; i < rescount; i++)
				{
					res->GetResDesc(i, rdesc);
					if (constantTable->textures.find(rdesc.resName) != constantTable->textures.end())
					{
						tex = constantTable->textures.at(rdesc.resName);
						tex->Apply(rdesc.offset);
					}
					else if (rdesc.isGlobal && sGlobalShaderConstants->textures.find(rdesc.resName) != sGlobalShaderConstants->textures.end())
					{
						tex = sGlobalShaderConstants->textures.at(rdesc.resName);
						tex->Apply(rdesc.offset);
					}
				}
			}
		}
	}
}

void DShader::ApplyStates(int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return;
		pass->ApplyStates();
	}
}

void DShader::Draw(int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return;
		DShaderRes* res = pass->GetShaderRes();
		if (res != NULL)
			res->Draw();
	}
}

int DShader::GetVertexUsage(int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return 0;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return 0;
		DShaderRes* res = pass->GetShaderRes();
		if (res != NULL)
			return res->GetVertexUsage();
	}
	return 0;
}

int DShader::GetPassCount()
{
	if (m_shaderBlock != NULL)
		return m_shaderBlock->GetPassCount();
	return 0;
}

//void DShader::ApplyParams(int cindex, int coffset, int csize, int stype, float * params) const
//{
//	if (m_shaderRes != NULL)
//		m_shaderRes->ApplyParams(cindex, coffset, csize, stype, params);
//}
//
//void DShader::ApplyRes(const LPCSTR key, DTexture *texture) const
//{
//	if (m_shaderRes != NULL && texture != NULL)
//	{
//		UINT offset = m_shaderRes->GetResOffset(key);
//		if (offset != NAN)
//		{
//			texture->Apply(offset);
//		}
//	}
//}

//void DShader::Apply(DShaderConstantTable* constantTable)
//{
//	if (m_shaderRes != NULL && constantTable != nullptr) {
//		if (sGlobalShaderConstants == nullptr)
//			sGlobalShaderConstants = new DShaderConstantTable();
//		m_shaderRes->ApplyParams(constantTable->params, sGlobalShaderConstants->params);
//
//		unsigned int rescount = m_shaderRes->GetResCount();
//		if (rescount > 0)
//		{
//			int i;
//			DShaderResDesc rdesc;
//			DTexture* tex;
//			for (i = 0; i < rescount; i++)
//			{
//				m_shaderRes->GetResDesc(i, rdesc);
//				if (constantTable->textures.find(rdesc.resName) != constantTable->textures.end())
//				{
//					tex = constantTable->textures.at(rdesc.resName);
//					tex->Apply(rdesc.offset);
//				}
//				else if (rdesc.isGlobal && sGlobalShaderConstants->textures.find(rdesc.resName) != sGlobalShaderConstants->textures.end())
//				{
//					tex = sGlobalShaderConstants->textures.at(rdesc.resName);
//					tex->Apply(rdesc.offset);
//				}
//			}
//		}
//	}
//}

//void DShader::Draw()
//{
//	if (m_shaderRes != NULL)
//		m_shaderRes->Draw();
//}

//int DShader::GetVertexUsage()
//{
//	if (m_shaderRes != NULL)
//		return m_shaderRes->GetVertexUsage();
//	return 0;
//}

//unsigned int DShader::GetPropertyCount(const LPCSTR cbufferName) const
//{
//	return m_shaderBuffer->GetPropertyCount(cbufferName);
//}
//
//int DShader::GetCBufferIndex(LPCSTR cbufferName) const
//{
//	return m_shaderBuffer->GetCBufferIndex(cbufferName);
//}
//
//int DShader::GetCBufferOffset(LPCSTR cbuffername) const
//{
//	return m_shaderBuffer->GetCBufferOffset(cbuffername);
//}
//
//int DShader::GetCBufferType(const LPCSTR cbufferName) const
//{
//	return m_shaderBuffer->GetCBufferType(cbufferName);
//}
//
//int DShader::GetPropertyIndex(const LPCSTR cbufferName, const LPCSTR key) const
//{
//	return m_shaderBuffer->GetPropertyIndex(cbufferName, key);
//}
//
//int DShader::GetPropertyOffset(const LPCSTR cbufferName, const LPCSTR key) const
//{
//	return m_shaderBuffer->GetPropertyOffset(cbufferName, key);
//}
//
//int DShader::GetPropertyLength(const LPCSTR cbufferName, const LPCSTR key) const
//{
//	return m_shaderBuffer->GetPropertyLength(cbufferName, key);
//}
//
//void DShader::GetPropertyInfo(const LPCSTR cbufferName, const LPCSTR key, int & index, int & offset, int & size) const
//{
//	m_shaderBuffer->GetPropertyInfo(cbufferName, key, index, offset, size);
//}
//
//void DShader::GetCBufferInfo(LPCSTR cbufferName, int & index, int & offset, int&length, int & shadertype) const
//{
//	m_shaderBuffer->GetCBufferInfo(cbufferName, index, offset, length, shadertype);
//}

//DShaderBuffer * DShader::GetShaderBuffer()
//{
//	return m_shaderBuffer;
//}

//DShaderParam * DShader::GetParam()
//{
//	return m_shaderBuffer->GetParams();
//}

