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

int DShader::GetCBufferIndex(LPCSTR cbufferName) const
{
	return m_shaderBuffer->GetCBufferIndex(cbufferName);
}

DShaderBuffer * DShader::GetShaderBuffer()
{
	return m_shaderBuffer;
}

DShaderParam * DShader::GetParam()
{
	return m_shaderBuffer->GetParams();
}
