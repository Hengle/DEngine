#include "DShader.h"
#include "DLog.h"
#include "DSystem.h"

DShader::DShader()
{
	m_shaderBuffer = 0;
}

DShader::~DShader()
{
}

DShader * DShader::Create(WCHAR * vertexShader, WCHAR * pixelShader)
{
	DShaderBuffer* sbf = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderBuffer(vertexShader, pixelShader);
	DShader* shader = new DShader();
	shader->m_shaderBuffer = sbf;
	return shader;
}

void DShader::Destroy()
{
	if (m_shaderBuffer != NULL)
	{
		m_shaderBuffer->Release();
		delete m_shaderBuffer;
		m_shaderBuffer = NULL;
	}
}

unsigned int DShader::GetCBufferCount() const
{ 
	return m_shaderBuffer->GetCBufferCount();
}

unsigned int DShader::GetPropertyCount() const
{
	return m_shaderBuffer->GetPropertyCount();
}

void DShader::GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int & poffset, int & plength, int & stype) const
{
	m_shaderBuffer->GetPropertyInfo(key, cindex, coffset, clength, poffset, plength, stype);
}

bool DShader::HasProperty(const LPCSTR key) const
{
	return m_shaderBuffer->HasProperty(key);
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

DShaderBuffer * DShader::GetShaderBuffer()
{
	return m_shaderBuffer;
}

//DShaderParam * DShader::GetParam()
//{
//	return m_shaderBuffer->GetParams();
//}
