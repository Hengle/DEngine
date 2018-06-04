#include "DMaterial.h"
#include "DSystem.h"


DMaterial::DMaterial(DShader * shader)
{
	m_shader = shader;
	//m_params = m_shader->GetParam();
	m_params = new MaterialParam*[m_shader->GetCBufferCount()]{ NULL };
	m_paramCount = m_shader->GetCBufferCount();
	//m_cbuffers = new void*[m_shader->GetCBufferCount()];
}

DMaterial::~DMaterial()
{
}

DShader * DMaterial::GetShader() const
{
	return m_shader;
}

void DMaterial::SetMatrix(const LPCSTR key, DMatrix4x4 & matrix)
{
	int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);

	int i;

	if (offset >= 0 && length == 16)
	{
		if (m_params[cindex] == NULL)
		{
			m_params[cindex] = new MaterialParam(clength, cindex, coffset, ctype);
		}
		
		for (i = 0; i < 16; i++)
		{
			m_params[cindex]->SetParam(offset+i, matrix[i]);
		}
	}
}

void DMaterial::SetVector4(const LPCSTR key, DVector4 & vector)
{
	int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);

	int i;

	if (offset >= 0 && length == 4)
	{
		if (m_params[cindex] == NULL)
		{
			m_params[cindex] = new MaterialParam(clength, cindex, coffset, ctype);
		}

		for (i = 0; i < 4; i++)
		{
			m_params[cindex]->SetParam(offset + i, vector[i]);
		}
	}
}

void DMaterial::SetVector3(const LPCSTR key, DVector3 & vector)
{
	int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);

	int i;

	if (offset >= 0 && length == 3)
	{
		if (m_params[cindex] == NULL)
		{
			m_params[cindex] = new MaterialParam(clength, cindex, coffset, ctype);
		}

		for (i = 0; i < 3; i++)
		{
			m_params[cindex]->SetParam(offset + i, vector[i]);
		}
	}
}

void DMaterial::SetVector2(const LPCSTR key, DVector2 & vector)
{
	int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);

	int i;

	if (offset >= 0 && length == 2)
	{
		if (m_params[cindex] == NULL)
		{
			m_params[cindex] = new MaterialParam(clength, cindex, coffset, ctype);
		}

		for (i = 0; i < 2; i++)
		{
			m_params[cindex]->SetParam(offset + i, vector[i]);
		}
	}
}

void DMaterial::SetColor(const LPCSTR key, DColor & color)
{
	int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);

	int i;

	if (offset >= 0 && length == 4)
	{
		if (m_params[cindex] == NULL)
		{
			m_params[cindex] = new MaterialParam(clength, cindex, coffset, ctype);
		}

		for (i = 0; i < 4; i++)
		{
			m_params[cindex]->SetParam(offset + i, color[i]);
		}
	}
}

void DMaterial::SetFloat(const LPCSTR key, float value)
{
	int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);

	if (offset >= 0 && length == 1)
	{
		if (m_params[cindex] == NULL)
		{
			m_params[cindex] = new MaterialParam(clength, cindex, coffset, ctype);
		}

		m_params[cindex]->SetParam(offset, value);
	}
}

//void DMaterial::SetTexture(const LPCSTR cbuffername, const LPCSTR key, const DTexture &)
//{
//}

//bool DMaterial::HasCBuffer(const LPCSTR buffername) const
//{
//	return m_shader->GetCBufferIndex(buffername) >= 0;
//}
//
//bool DMaterial::HasProperty(const LPCSTR buffername, const LPCSTR key) const
//{
//	return m_shader->GetPropertyIndex(buffername, key) >= 0;
//}

bool DMaterial::HasProperty(const LPCSTR key) const
{
	return m_shader->HasProperty(key);
}

int DMaterial::GetParamCount() const
{
	return m_paramCount;
}

void DMaterial::GetParams(int index, int & pcount, int& pindex, int & poffset, int & stype, float ** params)
{
	m_params[index]->GetParams(pcount, pindex, poffset, stype, params);
}

void DMaterial::Destroy()
{
	m_shader = NULL;
	//delete[] m_cbuffers;
	//m_cbuffers = NULL;
	//m_params->Release();
	//delete m_params;
	int i;
	for (i = 0; i < m_paramCount; i++)
	{
		if (m_params[i] != NULL)
		{
			m_params[i]->Release();
			delete m_params[i];
		}
		m_params[i] = NULL;
	}
	delete[] m_params;

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

DMaterial::MaterialParam::MaterialParam(int length, int index, int offset, int shadertype)
{
	m_params = new float[length];
	m_shaderType = shadertype;
	m_offset = offset;
	m_length = length;
	m_index = index;
}

DMaterial::MaterialParam::~MaterialParam()
{
}

void DMaterial::MaterialParam::SetParam(int index, float value)
{
	m_params[index] = value;
}

void DMaterial::MaterialParam::Release()
{
	delete[] m_params;
	m_params = NULL;
}

void DMaterial::MaterialParam::GetParams(int & pcount, int & pindex, int & poffset, int & stype, float ** params)
{
	pcount = m_length;
	pindex = m_index;
	poffset = m_offset;
	stype = m_shaderType;
	(*params) = m_params;
}