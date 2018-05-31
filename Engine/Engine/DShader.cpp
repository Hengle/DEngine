#include "DShader.h"
#include "DLog.h"
#include "DSystem.h"

DShader::DShader()
{
	m_ShaderBuffer = 0;
}

DShader::~DShader()
{
}

DShader * DShader::Create(WCHAR * vertexShader, WCHAR * pixelShader)
{
	DShaderBuffer* sbf = DSystem::GetGraphicsMgr()->GetGLCore()->CreateShaderBuffer(vertexShader, pixelShader);
	DShader* shader = new DShader();
	shader->m_ShaderBuffer = sbf;
	return shader;
}

void DShader::Destroy()
{
	if (m_ShaderBuffer != NULL)
	{
		m_ShaderBuffer->Release();
		delete m_ShaderBuffer;
		m_ShaderBuffer = NULL;
	}
}

unsigned int DShader::GetCBufferCount() const
{ 
	return m_ShaderBuffer->GetCBufferCount();
}

int DShader::GetCBufferIndex(LPCSTR cbufferName) const
{
	return m_ShaderBuffer->GetCBufferIndex(cbufferName);
}
