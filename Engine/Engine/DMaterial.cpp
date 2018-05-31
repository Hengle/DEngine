#include "DMaterial.h"



DMaterial::DMaterial(DShader * shader)
{
	m_shader = shader;
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
	return false;
}

DShader * DMaterial::GetShader()
{
	return nullptr;
}
