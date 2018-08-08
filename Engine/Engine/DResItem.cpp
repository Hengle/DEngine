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

DTextureCubeResItem::DTextureCubeResItem()
{
	m_rightGroupId = -1; m_rightId = -1;
	m_leftGroupId = -1; m_leftId = -1;
	m_topGroupId = -1; m_topId = -1;
	m_bottomGroupId = -1; m_bottomId = -1;
	m_frontGroupId = -1; m_frontId = -1;
	m_backGroupId = -1; m_backId = -1;
}

void DTextureCubeResItem::SetFace(int face, int groupId, int resId)
{
	if (face == 0)
	{
		m_rightGroupId = groupId;
		m_rightId = resId;
	}
	else if (face == 1)
	{
		m_leftGroupId = groupId;
		m_leftId = resId;
	}
	else if (face == 2)
	{
		m_topGroupId = groupId;
		m_topId = resId;
	}
	else if (face == 3)
	{
		m_bottomGroupId = groupId;
		m_bottomId = resId;
	}
	else if (face == 4)
	{
		m_frontGroupId = groupId;
		m_frontId = resId;
	}
	else if (face == 5)
	{
		m_backGroupId = groupId;
		m_backId = resId;
	}
}

void DTextureCubeResItem::Release()
{
	DResItem::Release();
}

DTextureCubeResItem * DTextureCubeResItem::LoadManifest(std::ifstream & ifile)
{
	DTextureCubeResItem* item = NULL;
	char face[32];
	int rightGid = -1, rightId = -1;
	int leftGid = -1, leftId = -1;
	int topGid = -1, topId = -1;
	int bottomGid = -1, bottomId = -1;
	int frontGid = -1, frontId = -1;
	int backGid = -1, backId = -1;
	while (!ifile.eof())
	{
		ifile >> face;
		if (strcmp(face, "#RIGHT") == 0)
		{
			ifile >> rightGid >> rightId;
		}
		else if (strcmp(face, "#LEFT") == 0)
		{
			ifile >> leftGid >> leftId;
		}
		else if (strcmp(face, "#TOP") == 0)
		{
			ifile >> topGid >> topId;
		}
		else if (strcmp(face, "#BOTTOM") == 0)
		{
			ifile >> bottomGid >> bottomId;
		}
		else if (strcmp(face, "#FRONT") == 0)
		{
			ifile >> frontGid >> frontId;
		}
		else if (strcmp(face, "#BACK") == 0)
		{
			ifile >> backGid >> backId;
		}
		else if (strcmp(face, "#RES_END") == 0)
		{
			if (rightGid >= 0 && rightId >= 0 && 
				leftGid >= 0 && leftId >= 0 &&
				topGid >= 0 && topId >= 0 &&
				bottomGid >= 0 && bottomId >= 0 &&
				frontGid >= 0 && frontId >= 0 &&
				backGid >= 0 && backId >= 0)
			{
				item = new DTextureCubeResItem();
				item->SetFace(0, rightGid, rightId);
				item->SetFace(1, leftGid, leftId);
				item->SetFace(2, topGid, topId);
				item->SetFace(3, bottomGid, bottomId);
				item->SetFace(4, frontGid, frontId);
				item->SetFace(5, backGid, backId);
			}
			return item;
		}
	}
	return item;
}

DResObject * DTextureCubeResItem::OnLoad()
{
	DTexture2D* right = DRes::Load<DTexture2D>(m_rightGroupId, m_rightId);
	DTexture2D* left = DRes::Load<DTexture2D>(m_leftGroupId, m_leftId);
	DTexture2D* top = DRes::Load<DTexture2D>(m_topGroupId, m_topId);
	DTexture2D* bottom = DRes::Load<DTexture2D>(m_bottomGroupId, m_bottomId);
	DTexture2D* front = DRes::Load<DTexture2D>(m_frontGroupId, m_frontId);
	DTexture2D* back = DRes::Load<DTexture2D>(m_backGroupId, m_backId);

	DTextureCube* cubemap = DTextureCube::Create(right, left, top, bottom, front, back);
	return cubemap;
}

//DMaterialResItem::DMaterialResItem(unsigned int shaderGroupId, unsigned int shaderId)
//{
//	m_shaderId = shaderId;
//	m_shaderGroupId = shaderGroupId;
//	m_path = 0;
//}

//void DMaterialResItem::SetPath(char * path)
//{
//	int len = strlen(path) + 1;
//	m_path = new char[len];
//	strcpy_s(m_path, len, path);
//}

//void DMaterialResItem::AddTexture(char * key, unsigned int textureGroupId, unsigned int textureId)
//{
//	if (m_textureIds.find(key) != m_textureIds.end())
//	{
//		m_textureIds[key].resId = textureId;
//		m_textureIds[key].groupId = textureGroupId;
//	}
//	else {
//		MaterialResTexDesc desc;
//		desc.groupId = textureGroupId;
//		desc.resId = textureId;
//		m_textureIds.insert(std::pair<std::string, MaterialResTexDesc>(key, desc));
//	}
//}

DMaterialResItem::DMaterialResItem(char * path)
{
	int len = strlen(path) + 1;
	m_path = new char[len];
	strcpy_s(m_path, len, path);
}

void DMaterialResItem::Release()
{
	DResItem::Release();
	//m_textureIds.clear();
	if (m_path != 0)
	{
		delete[] m_path;
		m_path = 0;
	}
}

DMaterialResItem * DMaterialResItem::LoadManifest(std::ifstream & ifile)
{
	char mdef[32], pname[256];
	//unsigned int texid, texgid, shaderid, shadergid;
	DMaterialResItem* item = NULL;
	while (!ifile.eof())
	{
		ifile >> mdef;
		if (strcmp(mdef, "#PATH") == 0)
		{
			ifile >> pname;
			item = new DMaterialResItem(pname);
		}
		/*if (strcmp(mdef, "#SHADER") == 0)
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
		}*/
		else if (strcmp(mdef, "#RES_END") == 0)
		{
			return item;
		}
	}
	return NULL;
}

DResObject * DMaterialResItem::OnLoad()
{
	DResObject* mat = LoadMaterial();
	return mat;
	/*DShader* shader = DRes::Load<DShader>(m_shaderGroupId, m_shaderId);
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
	return obj;*/
}

DResObject * DMaterialResItem::LoadMaterial()
{
	if (m_path == 0)
		return NULL;
	std::ifstream ifile;
	ifile.open(m_path);
	if (ifile.fail())
	{
		return NULL;
	}

	char ptype[64];

	DResObject* mat = NULL;

	while (!ifile.eof())
	{
		ifile >> ptype;
		if (strcmp(ptype, "#SHADER") == 0)
		{
			LoadShader(ifile, &mat);
		}
		else if(strcmp(ptype, "#PARAM_TEX") == 0)
		{
			LoadTexture(ifile, mat);
		}
		else if (strcmp(ptype, "#PARAM_FLOAT") == 0)
		{
			LoadNumbers(ifile, mat, 1);
		}
		else if (strcmp(ptype, "#PARAM_VECTOR2") == 0)
		{
			LoadNumbers(ifile, mat, 2);
		}
		else if (strcmp(ptype, "#PARAM_VECTOR3") == 0)
		{
			LoadNumbers(ifile, mat, 3);
		}
		else if (strcmp(ptype, "#PARAM_VECTOR4") == 0)
		{
			LoadNumbers(ifile, mat, 4);
		}
	}
	ifile.close();

	return mat;
}

void DMaterialResItem::LoadShader(std::ifstream& ifile, DResObject ** mat)
{
	unsigned int shaderid, shadergid;
	ifile >> shadergid >> shaderid;
	DShader* shader = DRes::Load<DShader>(shadergid, shaderid);
	if (shader == NULL)
		return;
	*mat = new DMaterial(shader);
}

void DMaterialResItem::LoadTexture(std::ifstream & ifile, DResObject * mat)
{
	unsigned int texid, texgid;
	char texname[64];
	ifile >> texname >> texgid >> texid;
	if (mat != NULL)
	{
		DTexture* tex = DRes::Load<DTexture>(texgid, texid);
		if (tex != NULL)
		{
			((DMaterial*)mat)->SetTexture(texname, tex);
		}
	}
}

void DMaterialResItem::LoadNumbers(std::ifstream & ifile, DResObject * mat, int count)
{
	if (count <= 0 || count > 4)
		return;
	if (mat == NULL)
		return;
	char pname[32];
	float v0, v1, v2, v3;
	if (count == 1)
	{
		ifile >> pname >> v0;
		((DMaterial*)mat)->SetFloat(pname, v0);
	}
	else if (count == 2)
	{
		ifile >> pname >> v0 >> v1;
		((DMaterial*)mat)->SetVector2(pname, DVector2(v0, v1));
	}
	else if (count == 3)
	{
		ifile >> pname >> v0 >> v1 >> v2;
		((DMaterial*)mat)->SetVector3(pname, DVector3(v0, v1, v2));
	}
	else if (count == 4)
	{
		ifile >> pname >> v0 >> v1 >> v2 >> v3;
		((DMaterial*)mat)->SetVector4(pname, DVector4(v0, v1, v2, v3));
	}
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

DInternalResItem::DInternalResItem(unsigned int resid)
{
	m_resId = resid;
}

DResObject * DInternalResItem::OnLoad()
{
	//TODO 暂时直接这样创建内部资源
	if (m_resId == D_RES_PLANE)
		return DGeometry::Create(DGeometry_Plane);
	if (m_resId == D_RES_SPHERE)
		return DGeometry::Create(DGeometry_Sphere);
	return nullptr;
}
