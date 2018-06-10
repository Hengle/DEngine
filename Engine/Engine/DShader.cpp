#include "DShader.h"
#include "DLog.h"
#include "DSystem.h"

DShader::DShader()
{
	m_shaderRes = 0;
}

DShader::~DShader()
{
}

DShader * DShader::Create(WCHAR * vertexShader, WCHAR * pixelShader)
{
	DShaderRes* sbf = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderRes();
	if (sbf != NULL)
	{
		sbf->Init(vertexShader, pixelShader);
		DShader* shader = new DShader();
		shader->m_shaderRes = sbf;
		return shader;
	}
	return NULL;
}

void DShader::Destroy()
{
	if (m_shaderRes != NULL)
	{
		m_shaderRes->Release();
		delete m_shaderRes;
		m_shaderRes = NULL;
	}
}

unsigned int DShader::GetCBufferCount() const
{
	if (m_shaderRes != NULL)
		return m_shaderRes->GetCBufferCount();
	return 0;
}

unsigned int DShader::GetPropertyCount() const
{
	if (m_shaderRes != NULL)
		return m_shaderRes->GetPropertyCount();
	return 0;
}

void DShader::GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const
{
	if (m_shaderRes != NULL)
		m_shaderRes->GetPropertyInfo(key, desc);
}

bool DShader::HasProperty(const LPCSTR key) const
{
	if (m_shaderRes != NULL)
		return m_shaderRes->HasProperty(key);
}

void DShader::ApplyParams(int cindex, int coffset, int csize, int stype, float * params) const
{
	if (m_shaderRes != NULL)
		m_shaderRes->ApplyParams(cindex, coffset, csize, stype, params);
}

void DShader::Draw(int indexCount)
{
	if (m_shaderRes != NULL)
		m_shaderRes->Draw(indexCount);
}

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
