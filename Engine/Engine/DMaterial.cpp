#include "DMaterial.h"
#include "DSystem.h"


DMaterial::DMaterial(DShader * shader)
{
	m_shader = shader;
	m_params = m_shader->GetParam();
	//m_cbuffers = new void*[m_shader->GetCBufferCount()];
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

void DMaterial::Destroy()
{
	m_shader = NULL;
	//delete[] m_cbuffers;
	//m_cbuffers = NULL;
	m_params->Release();
	delete m_params;
	m_params = NULL;
}

//void DMaterial::SetParams()
//{
//	
//	int cbcount = m_shader->GetCBufferCount();
//	int i;
//	for (i = 0; i < cbcount; i++)
//	{
//
//	}
//}


