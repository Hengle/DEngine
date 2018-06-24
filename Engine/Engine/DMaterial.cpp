#include "DMaterial.h"
#include "DSystem.h"


DMaterial::DMaterial(DShader * shader)
{
	m_shader = shader;
	//m_params = m_shader->GetParam();
	m_params = new MaterialParam*[m_shader->GetCBufferCount()]{ NULL };
	m_paramCount = m_shader->GetCBufferCount();
	m_cullMode = DCullMode_Back;
	m_ztest = DRSCompareFunc_LEqual;
	m_zwrite = true;
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
	if (m_shader == NULL)
		return;
	DShaderParamDesc desc;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	//m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);
	m_shader->GetPropertyInfo(key, &desc);

	int i;

	if (desc.propertyOffset >= 0 && desc.propertySize == 16)
	{
		if (m_params[desc.cbufferIndex] == NULL)
		{
			m_params[desc.cbufferIndex] = new MaterialParam(desc.propertySize, desc.cbufferIndex, desc.cbufferLength, desc.cbufferOffset, desc.shaderType);
		}
		
		for (i = 0; i < 16; i++)
		{
			m_params[desc.cbufferIndex]->SetParam(desc.propertyOffset+i, matrix[i]);
		}
	}
}

void DMaterial::SetVector4(const LPCSTR key, DVector4 & vector)
{
	if (m_shader == NULL)
		return;
	DShaderParamDesc desc;
	//int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	//m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);
	m_shader->GetPropertyInfo(key, &desc);

	int i;

	if (desc.propertyOffset >= 0 && desc.propertySize == 4)
	{
		if (m_params[desc.cbufferIndex] == NULL)
		{
			m_params[desc.cbufferIndex] = new MaterialParam(desc.propertySize, desc.cbufferIndex, desc.cbufferLength, desc.cbufferOffset, desc.shaderType);
		}

		for (i = 0; i < 4; i++)
		{
			m_params[desc.cbufferIndex]->SetParam(desc.propertyOffset + i, vector[i]);
		}
	}
}

void DMaterial::SetVector3(const LPCSTR key, DVector3 & vector)
{
	if (m_shader == NULL)
		return;
	DShaderParamDesc desc;
	//int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	//m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);
	m_shader->GetPropertyInfo(key, &desc);

	int i;

	if (desc.propertyOffset >= 0 && desc.propertySize == 3)
	{
		if (m_params[desc.cbufferIndex] == NULL)
		{
			m_params[desc.cbufferIndex] = new MaterialParam(desc.propertySize, desc.cbufferIndex, desc.cbufferLength, desc.cbufferOffset, desc.shaderType);
		}

		for (i = 0; i < 3; i++)
		{
			m_params[desc.cbufferIndex]->SetParam(desc.propertyOffset + i, vector[i]);
		}
	}
}

void DMaterial::SetVector2(const LPCSTR key, DVector2 & vector)
{
	if (m_shader == NULL)
		return;
	DShaderParamDesc desc;
	//int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	//m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);
	m_shader->GetPropertyInfo(key, &desc);

	int i;

	if (desc.propertyOffset >= 0 && desc.propertySize == 2)
	{
		if (m_params[desc.cbufferIndex] == NULL)
		{
			m_params[desc.cbufferIndex] = new MaterialParam(desc.propertySize, desc.cbufferIndex, desc.cbufferLength, desc.cbufferOffset, desc.shaderType);
		}

		for (i = 0; i < 2; i++)
		{
			m_params[desc.cbufferIndex]->SetParam(desc.propertyOffset + i, vector[i]);
		}
	}
}

void DMaterial::SetColor(const LPCSTR key, DColor & color)
{
	if (m_shader == NULL)
		return;
	DShaderParamDesc desc;
	//int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	//m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);
	m_shader->GetPropertyInfo(key, &desc);

	int i;

	if (desc.propertyOffset >= 0 && desc.propertySize == 4)
	{
		if (m_params[desc.cbufferIndex] == NULL)
		{
			m_params[desc.cbufferIndex] = new MaterialParam(desc.propertySize, desc.cbufferIndex, desc.cbufferLength, desc.cbufferOffset, desc.shaderType);
		}

		for (i = 0; i < 4; i++)
		{
			m_params[desc.cbufferIndex]->SetParam(desc.propertyOffset + i, color[i]);
		}
	}
}

void DMaterial::SetFloat(const LPCSTR key, float value)
{
	if (m_shader == NULL)
		return;
	DShaderParamDesc desc;
	//int offset, length, cindex, coffset, ctype, clength;
	//m_shader->GetCBufferInfo(cbuffername, cindex, coffset, clength, ctype);
	//m_shader->GetPropertyInfo(key, cindex, coffset, clength, offset, length, ctype);
	m_shader->GetPropertyInfo(key, &desc);

	if (desc.propertyOffset >= 0 && desc.propertySize == 1)
	{
		if (m_params[desc.cbufferIndex] == NULL)
		{
			m_params[desc.cbufferIndex] = new MaterialParam(desc.propertySize, desc.cbufferIndex, desc.cbufferLength, desc.cbufferOffset, desc.shaderType);
		}

		m_params[desc.cbufferIndex]->SetParam(desc.propertyOffset, value);
	}
}

void DMaterial::SetTexture(const LPCSTR key, DTexture * texture)
{
	if (m_shader == NULL)
		return;
	if (m_textures.find(key) == m_textures.end())
	{
		m_textures.insert(std::pair<std::string, DTexture*>(key, texture));
	}
	else
	{
		m_textures[key] = texture;
	}
}

void DMaterial::SetCullMode(DCullMode cullMode)
{
	m_cullMode = cullMode;
}

void DMaterial::SetZWrite(bool zwrite)
{
	m_zwrite = zwrite;
}

void DMaterial::SetZTest(DRSCompareFunc ztest)
{
	m_ztest = ztest;
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
	if (m_shader == NULL)
		return false;
	return m_shader->HasProperty(key);
}

void DMaterial::Apply()
{
	if (m_shader == NULL)
		return;
	int i = 0;
	int pcount, poffset, pindex, psize, stype;
	float* params;

	DGraphics::SetCullMode(m_cullMode);
	DGraphics::SetZTestFunc(m_ztest);
	DGraphics::SetZWriteEnable(m_zwrite);

	for (i = 0; i < m_paramCount; i++)
	{
		//material->GetParams(i, pcount, pindex, poffset, psize, stype, &params);
		m_params[i]->GetParams(pcount, pindex, poffset, psize, stype, &params);
		m_shader->ApplyParams(pindex, poffset, psize, stype, params);
		//DSystem::GetGraphicsMgr()->GetGLCore()->ApplyShaderParams(material->GetShader()->GetShaderBuffer(), pindex, poffset, psize, stype, params);
	}

	std::map<std::string, DTexture*>::iterator  iter;
	for (iter = m_textures.begin(); iter != m_textures.end(); iter++)
	{
		if (iter->second != NULL)
		{
			LPCSTR key = iter->first.c_str();
			m_shader->ApplyRes(key, iter->second);
		}
	}

	m_shader->Draw();
	//DSystem::GetGraphicsMgr()->GetGLCore()->DrawShader(material->GetShader()->GetShaderBuffer(), mesh->GetIndexCount());
}

//int DMaterial::GetParamCount() const
//{
//	return m_paramCount;
//}
//
//void DMaterial::GetParams(int index, int & pcount, int& pindex, int & poffset, int& psize, int & stype, float ** params)
//{
//	m_params[index]->GetParams(pcount, pindex, poffset, psize, stype, params);
//}

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

	std::map<std::string, DTexture*>::iterator  iter;
	for (iter = m_textures.begin(); iter != m_textures.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Destroy();
			delete iter->second;
		}
	}

	m_textures.clear();

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

DMaterial::MaterialParam::MaterialParam(int length, int index, int size, int offset, int shadertype)
{
	m_params = new float[size];
	m_shaderType = shadertype;
	m_offset = offset;
	m_length = length;
	m_index = index;
	m_size = size;
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

void DMaterial::MaterialParam::GetParams(int & pcount, int & pindex, int & poffset, int & psize, int & stype, float ** params)
{
	pcount = m_length;
	pindex = m_index;
	poffset = m_offset;
	psize = m_size;
	stype = m_shaderType;
	(*params) = m_params;
}