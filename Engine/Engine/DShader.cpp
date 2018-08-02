#include "DShader.h"
#include "DLog.h"
#include "DSystem.h"

void DShaderConstantTable::Release()
{
	std::map<std::string, float*>::iterator piter;
	for (piter = params.begin(); piter != params.end(); piter++)
	{
		if (piter->second != NULL)
		{
			delete[] piter->second;
		}
	}
	params.clear();

	std::map<std::string, DTexture*>::iterator  titer;
	for (titer = textures.begin(); titer != textures.end(); titer++)
	{
		if (titer->second != NULL)
		{
			titer->second->Destroy();
			delete titer->second;
		}
	}

	textures.clear();
}

void DShaderConstantTable::SetMatrix(const LPCSTR key, DMatrix4x4 & matrix)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[16];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 16; i++)
	{
		values[i] = matrix[i];
	}
}

void DShaderConstantTable::SetVector4(const LPCSTR key, DVector4 & vector)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[4];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 4; i++)
	{
		values[i] = vector[i];
	}
}

void DShaderConstantTable::SetVector3(const LPCSTR key, DVector3 & vector)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[3];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 3; i++)
	{
		values[i] = vector[i];
	}
}

void DShaderConstantTable::SetVector2(const LPCSTR key, DVector2 & vector)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[2];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 2; i++)
	{
		values[i] = vector[i];
	}
}

void DShaderConstantTable::SetColor(const LPCSTR key, DColor & color)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[4];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	for (i = 0; i < 4; i++)
	{
		values[i] = color[i];
	}
}

void DShaderConstantTable::SetFloat(const LPCSTR key, float value)
{
	float* values = 0;
	int i;
	if (params.find(key) != params.end())
	{
		values = params.at(key);
	}
	else
	{
		values = new float[1];
		params.insert(std::pair<std::string, float*>(key, values));
	}

	values[0] = value;
}

void DShaderConstantTable::SetTexture(const LPCSTR key, DTexture * texture)
{
	if (textures.find(key) == textures.end())
	{
		textures.insert(std::pair<std::string, DTexture*>(key, texture));
	}
	else
	{
		textures[key] = texture;
	}
}

DShader::DShader()
{
	m_shaderBlock = 0;
}

DShader::~DShader()
{
}

DShader * DShader::Create(char * path)
{
	DShaderBlock* block = new DShaderBlock();
	block->Compile(path);
	if (!block->IsSupported())
	{
		block->Release();
		delete block;
		return NULL;
	}
	DShader* shader = new DShader();
	shader->m_shaderBlock = block;
	return shader;
}

void DShader::ReleaseGlobalConstants()
{
	if (sGlobalShaderConstants != nullptr)
	{
		sGlobalShaderConstants->Release();
		delete sGlobalShaderConstants;
		sGlobalShaderConstants = nullptr;
	}
}

void DShader::SetGlobalMatrix(const LPCSTR key, DMatrix4x4 & matrix)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetMatrix(key, matrix);
}

void DShader::SetGlobalVector4(const LPCSTR key, DVector4 & vector)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetVector4(key, vector);
}

void DShader::SetGlobalVector3(const LPCSTR key, DVector3 & vector)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetVector3(key, vector);
}

void DShader::SetGlobalVector2(const LPCSTR key, DVector2 & vector)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetVector2(key, vector);
}

void DShader::SetGlobalColor(const LPCSTR key, DColor & color)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetColor(key, color);
}

void DShader::SetGlobalFloat(const LPCSTR key, float value)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetFloat(key, value);
}

void DShader::SetGlobalTexture(const LPCSTR key, DTexture * texture)
{
	if (sGlobalShaderConstants == nullptr)
		sGlobalShaderConstants = new DShaderConstantTable();
	sGlobalShaderConstants->SetTexture(key, texture);
}

void DShader::ApplyShaderProgramParams(DShaderProgram * program, DShaderConstantTable * constantTable)
{
	if (constantTable != nullptr) {
		if (sGlobalShaderConstants == nullptr)
			sGlobalShaderConstants = new DShaderConstantTable();
		program->ApplyParams(constantTable->params, sGlobalShaderConstants->params);

		unsigned int rescount = program->GetResCount();
		if (rescount > 0)
		{
			int i;
			DShaderResDesc rdesc;
			DTexture* tex;
			for (i = 0; i < rescount; i++)
			{
				program->GetResDesc(i, rdesc);
				if (constantTable->textures.find(rdesc.resName) != constantTable->textures.end())
				{
					tex = constantTable->textures.at(rdesc.resName);
					tex->Apply(rdesc.offset, i);
				}
				else if (rdesc.isGlobal && sGlobalShaderConstants->textures.find(rdesc.resName) != sGlobalShaderConstants->textures.end())
				{
					tex = sGlobalShaderConstants->textures.at(rdesc.resName);
					tex->Apply(rdesc.offset, i);
				}
			}
		}
	}
}

void DShader::Destroy()
{
	if (m_shaderBlock != NULL)
	{
		m_shaderBlock->Release();
		delete m_shaderBlock;
		m_shaderBlock = NULL;
	}
}

bool DShader::HasProperty(const LPCSTR key) const
{
	if (m_shaderBlock != NULL)
	{
		int scount,i;
		scount = m_shaderBlock->GetPassCount();
		DShaderPass* pass;
		//DShaderProgram* prog;
		for (i = 0; i < scount; i++)
		{
			pass = m_shaderBlock->GetPass(i);
			if (pass == NULL)
				continue;
			return pass->HasProperty(key);
			/*prog = pass->GetVertexShader();
			if (prog != NULL) {
				if (prog->HasProperty(key))
					return true;
			}
			prog = pass->GetPixelShader();
			if (prog != NULL) {
				if (prog->HasProperty(key))
					return true;
			}*/
		}
	}
	return false;
}

void DShader::ApplyParams(DShaderConstantTable * constantTable, int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return;
		if (sGlobalShaderConstants == nullptr)
			sGlobalShaderConstants = new DShaderConstantTable();
		//pass->ApplyParams(constantTable, sGlobalShaderConstants);
		int programCount = pass->GetShaderProgramCount();
		int i;
		DShaderProgram* program = 0;
		
		for (i = 0; i < programCount; i++)
		{
			program = pass->GetShaderProgram(i);
			if (program != NULL)
				ApplyShaderProgramParams(program, constantTable);
		}
		
	}
}

void DShader::ApplyStates(int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return;
		pass->ApplyStates();
	}
}

void DShader::Draw(int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return;
		pass->Draw();
		/*DShaderProgram* prog = pass->GetVertexShader();
		if (prog != NULL)
			prog->Draw();
		prog = pass->GetPixelShader();
		if (prog != NULL)
			prog->Draw();*/
	}
}

int DShader::GetVertexUsage(int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return 0;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return 0;
		return pass->GetVertexUsage();
		/*DShaderProgram* vprog = pass->GetVertexShader();
		if (vprog != NULL)
			return vprog->GetVertexUsage();*/
	}
	return 0;
}

int DShader::GetPassCount()
{
	if (m_shaderBlock != NULL)
		return m_shaderBlock->GetPassCount();
	return 0;
}

DRenderQueue DShader::GetRenderQueue()
{
	if (m_shaderBlock != NULL)
		return m_shaderBlock->GetRenderQueue();
	return DRenderQueue_Opaque;
}

bool DShader::IsPassEnable(int index)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return false;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return false;
		return pass->IsPassEnable();
	}
	return false;
}

void DShader::SetPassEnable(int index, bool enable)
{
	if (m_shaderBlock != NULL)
	{
		int passcount = m_shaderBlock->GetPassCount();
		if (index < 0 || index >= passcount)
			return;
		DShaderPass* pass = m_shaderBlock->GetPass(index);
		if (pass == NULL)
			return;
		pass->SetPassEnable(enable);
	}
}
