#include "DMaterial.h"
#include "DGraphics.h"

DMaterial::DMaterial(DShader * shader)
{
	m_shader = shader;
	m_constantTable = new DShaderConstantTable();
	m_currentPass = -1;
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
	if (m_constantTable == nullptr)
		m_constantTable = new DShaderConstantTable();
	m_constantTable->SetMatrix(key, matrix);
}

void DMaterial::SetVector4(const LPCSTR key, DVector4 & vector)
{
	if (m_shader == NULL)
		return;
	if (m_constantTable == nullptr)
		m_constantTable = new DShaderConstantTable();
	m_constantTable->SetVector4(key, vector);
}

void DMaterial::SetVector3(const LPCSTR key, DVector3 & vector)
{
	if (m_shader == NULL)
		return;
	if (m_constantTable == nullptr)
		m_constantTable = new DShaderConstantTable();
	m_constantTable->SetVector3(key, vector);

}

void DMaterial::SetVector2(const LPCSTR key, DVector2 & vector)
{
	if (m_shader == NULL)
		return;
	if (m_constantTable == nullptr)
		m_constantTable = new DShaderConstantTable();
	m_constantTable->SetVector2(key, vector);

}

void DMaterial::SetColor(const LPCSTR key, DColor & color)
{
	if (m_shader == NULL)
		return;
	if (m_constantTable == nullptr)
		m_constantTable = new DShaderConstantTable();
	m_constantTable->SetColor(key, color);
}

void DMaterial::SetFloat(const LPCSTR key, float value)
{
	if (m_shader == NULL)
		return;
	if (m_constantTable == nullptr)
		m_constantTable = new DShaderConstantTable();
	m_constantTable->SetFloat(key, value);

}

void DMaterial::SetTexture(const LPCSTR key, DTexture * texture)
{
	if (m_shader == NULL)
		return;
	if (m_constantTable == nullptr)
		m_constantTable = new DShaderConstantTable();
	m_constantTable->SetTexture(key, texture);

}

bool DMaterial::HasProperty(const LPCSTR key) const
{
	if (m_shader == NULL)
		return false;
	return m_shader->HasProperty(key);
}

void DMaterial::SetShader(DShader * shader)
{
	m_shader = shader;
}

DShader * DMaterial::GetShader()
{
	return m_shader;
}

bool DMaterial::SetPass(int pass)
{
	if (m_shader == NULL)
		return false;
	if (!IsPassEnable(pass))
		return false;
	
	m_currentPass = pass;
	DGraphics::SetActiveMaterial(this);
	/*m_shader->ApplyStates(pass);

	m_shader->ApplyParams(m_constantTable, pass);

	m_shader->Draw(pass);*/

	return true;
}

void DMaterial::ApplyPass()
{
	if (m_shader == NULL)
		return;
	if (!IsPassEnable(m_currentPass))
		return;

	m_shader->ApplyStates(m_currentPass);

	

	m_shader->Draw(m_currentPass);

	m_shader->ApplyParams(m_constantTable, m_currentPass);

	
}

bool DMaterial::IsPassEnable(int pass)
{
	if (m_shader != NULL)
		return m_shader->IsPassEnable(pass);
	return false;
}

void DMaterial::SetPassEnable(int pass, bool enable)
{
	if (m_shader != NULL)
		m_shader->SetPassEnable(pass, enable);
}

int DMaterial::GetVertexUsage(int pass)
{
	if (m_shader != NULL)
		return m_shader->GetVertexUsage(pass);
	return 0;
}

int DMaterial::GetPassCount()
{
	if (m_shader != NULL)
		return m_shader->GetPassCount();
	return 0;
}

DRenderQueue DMaterial::GetRenderQueue()
{
	if (m_shader != NULL)
		return m_shader->GetRenderQueue();
	return DRenderQueue_Opaque;
}

void DMaterial::Destroy()
{
	DGraphics::ClearActiveMaterial(this);

	m_shader = NULL;

	if (m_constantTable != nullptr)
	{
		m_constantTable->Release();
		delete m_constantTable;
		m_constantTable = nullptr;
	}

}