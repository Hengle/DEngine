#include "DResItem.h"
#include "DShader.h"
#include "DUtils.h"
#include "DSystem.h"

DResItem::DResItem()
{
	m_isLoaded = false;
}

void DResItem::Load()
{
	if (m_isLoaded)
		return;
	m_isLoaded = true;
	m_res = OnLoad();
}

void DResItem::Unload()
{
	if (!m_isLoaded)
		return;
	m_isLoaded = false;
	if (m_res != NULL)
	{
		m_res->Destroy();
		delete m_res;
		m_res = NULL;
	}
}

void DResItem::Release()
{
	Unload();
}

DResObject * DResItem::GetRes()
{
	return m_res;
}

DShaderResItem::DShaderResItem(char * path)
{
	m_path = path;
}

void DShaderResItem::Release()
{
	DResItem::Release();
	delete[] m_path;
}

DShaderResItem * DShaderResItem::LoadManifest(std::ifstream & ifile)
{
	char gdef[32], path[512];
	char* argp;
	int argplen;
	DShaderResItem* item = NULL;
	while (!ifile.eof())
	{
		ifile >> gdef;
		if (strcmp(gdef, "#PATH") == 0)
		{
			ifile >> path;
			argplen = strlen(path) + 1;
			argp = new char[argplen];
			strcpy_s(argp, argplen, path);
			item = new DShaderResItem(argp);
		}
		else if (strcmp(gdef, "#RES_END") == 0)
		{
			return item;
		}
	}
	return NULL;
}

DResObject * DShaderResItem::OnLoad()
{
	//wchar_t* p = CharToWCHAR(m_path);
	DResObject* obj = DShader::Create(m_path);
	//delete[] p;
	return obj;
}

DTexture2DResItem::DTexture2DResItem(char * path, int wrapMode)
{
	m_path = path;
	m_wrapMode = wrapMode;
}

void DTexture2DResItem::Release()
{
	DResItem::Release();
	delete[] m_path;
}

DTexture2DResItem * DTexture2DResItem::LoadManifest(std::ifstream & ifile)
{
	char tdef[32], path[512], wmode[32];
	char* argp = 0;
	int argplen;
	DTexture2DResItem* item = NULL;
	int wrapMode = 1;
	while (!ifile.eof())
	{
		ifile >> tdef;
		if (strcmp(tdef, "#PATH") == 0)
		{
			ifile >> path;
			argplen = strlen(path) + 1;
			argp = new char[argplen];
			strcpy_s(argp, argplen, path);
		}
		else if (strcmp(tdef, "#WRAPMODE") == 0)
		{
			ifile >> wmode;
			if (strcmp(wmode, "REPEAT") == 0)
			{
				wrapMode = 0;
			}
		}
		else if (strcmp(tdef, "#RES_END") == 0)
		{
			if (argp != 0)
			{
				item = new DTexture2DResItem(argp, wrapMode);
			}
			return item;
		}
	}
	return NULL;
}

DResObject * DTexture2DResItem::OnLoad()
{
	wchar_t* p = CharToWCHAR(m_path);
	DResObject* obj = DTexture2D::Create(p, (DWrapMode)m_wrapMode);
	delete[] p;
	return obj;
}

DMaterialResItem::DMaterialResItem(unsigned int shaderGroupId, unsigned int shaderId)
{
	m_shaderId = shaderId;
	m_shaderGroupId = shaderGroupId;
}

void DMaterialResItem::AddTexture(char * key, unsigned int textureGroupId, unsigned int textureId)
{
	if (m_textureIds.find(key) != m_textureIds.end())
	{
		m_textureIds[key].resId = textureId;
		m_textureIds[key].groupId = textureGroupId;
	}
	else {
		MaterialResTexDesc desc;
		desc.groupId = textureGroupId;
		desc.resId = textureId;
		m_textureIds.insert(std::pair<std::string, MaterialResTexDesc>(key, desc));
	}
}

void DMaterialResItem::Release()
{
	m_textureIds.clear();
}

DMaterialResItem * DMaterialResItem::LoadManifest(std::ifstream & ifile)
{
	char mdef[32], pname[32];
	unsigned int texid, texgid, shaderid, shadergid;
	DMaterialResItem* item = NULL;
	while (!ifile.eof())
	{
		ifile >> mdef;
		if (strcmp(mdef, "#SHADER") == 0)
		{
			ifile >> shadergid >> shaderid;
			item = new DMaterialResItem(shadergid, shaderid);
		}
		else if (strcmp(mdef, "#PARAM_TEX") == 0)
		{
			ifile >> pname >> texgid >> texid;
			if (item != NULL)
			{
				item->AddTexture(pname, texgid, texid);
			}
		}
		else if (strcmp(mdef, "#RES_END") == 0)
		{
			return item;
		}
	}
	return NULL;
}

DResObject * DMaterialResItem::OnLoad()
{
	DShader* shader = DRes::Load<DShader>(m_shaderGroupId, m_shaderId);
	if (shader == NULL)
		return NULL;
	DMaterial* obj = new DMaterial(shader);
	std::map<std::string, MaterialResTexDesc>::iterator iter;
	for (iter = m_textureIds.begin(); iter != m_textureIds.end(); iter++)
	{
		DTexture2D* tex = DRes::Load<DTexture2D>(iter->second.groupId, iter->second.resId);
		if (tex != NULL)
		{
			obj->SetTexture(iter->first.c_str(), tex);
		}
	}
	return obj;
}

DGeometryResItem::DGeometryResItem(char * path)
{
	m_path = path;
}

void DGeometryResItem::Release()
{
	DResItem::Release();
	delete[] m_path;
}

DGeometryResItem * DGeometryResItem::LoadManifest(std::ifstream & ifile)
{
	char mdef[32], path[512];
	char* argp = 0;
	int argplen;
	DGeometryResItem* item = NULL;
	while (!ifile.eof())
	{
		ifile >> mdef;
		if (strcmp(mdef, "#PATH") == 0)
		{
			ifile >> path;
			argplen = strlen(path) + 1;
			argp = new char[argplen];
			strcpy_s(argp, argplen, path);
		}
		else if (strcmp(mdef, "#RES_END") == 0)
		{
			if (argp != 0)
			{
				item = new DGeometryResItem(argp);
			}
			return item;
		}
	}
	return NULL;
}

DResObject * DGeometryResItem::OnLoad()
{
	return DGeometry::Create(m_path);
}
