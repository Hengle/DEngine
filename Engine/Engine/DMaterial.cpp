#include "DMaterial.h"



DMaterial::DMaterial(DShader * shader)
{
	m_shader = shader;
	m_cbuffers = new void*[m_shader->GetCBufferCount()];
}

DMaterial::~DMaterial()
{
}

DShader * DMaterial::GetShader() const
{
	return m_shader;
}

bool DMaterial::HasCBuffer(LPCSTR buffername)
{
	return m_shader->GetCBufferIndex(buffername) >= 0;
}

void DMaterial::Draw()
{
	SetParams();
	RenderShader();
}

void DMaterial::Destroy()
{
	m_shader = NULL;
	delete[] m_cbuffers;
}

void DMaterial::SetParams()
{
}

void DMaterial::RenderShader()
{
}

DShader * DMaterial::GetShader() const
{
	return m_shader;
}
