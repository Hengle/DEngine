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

DShaderResItem::DShaderResItem(char * vsfile, char * psfile)
{
	m_vsfile = vsfile;
	m_psfile = psfile;
}

void DShaderResItem::Release()
{
	DResItem::Release();
	delete[] m_vsfile;
	delete[] m_psfile;
}

DShaderResItem * DShaderResItem::LoadManifest(std::ifstream & ifile)
{
	char gdef[32], vspath[512], pspath[512];
	char* argvp, *argpp;
	int argvlen, argplen;
	DShaderResItem* item = NULL;
	while (!ifile.eof())
	{
		ifile >> gdef;
		if (strcmp(gdef, "#D3D9") == 0)
		{
			ifile >> vspath >> pspath;
			if (DSystem::GetGraphicsMgr()->GetAPI() == DGRAPHICS_API_D3D9)
			{
				argvlen = strlen(vspath) + 1;
				argvp = new char[argvlen];
				strcpy_s(argvp, argvlen, vspath);
				argplen = strlen(pspath) + 1;
				argpp = new char[argplen];
				strcpy_s(argpp, argplen, pspath);
				item = new DShaderResItem(argvp, argpp);
			}
		}
		else if (strcmp(gdef, "#D3D10") == 0)
		{
			ifile >> vspath >> pspath;
			if (DSystem::GetGraphicsMgr()->GetAPI() == DGRAPHICS_API_D3D10)
			{
				argvlen = strlen(vspath) + 1;
				argvp = new char[argvlen];
				strcpy_s(argvp, argvlen, vspath);
				argplen = strlen(pspath) + 1;
				argpp = new char[argplen];
				strcpy_s(argpp, argplen, pspath);
				item = new DShaderResItem(argvp, argpp);
			}
		}
		else if (strcmp(gdef, "#D3D11") == 0)
		{
			ifile >> vspath >> pspath;
			if (DSystem::GetGraphicsMgr()->GetAPI() == DGRAPHICS_API_D3D11)
			{
				argvlen = strlen(vspath) + 1;
				argvp = new char[argvlen];
				strcpy_s(argvp, argvlen, vspath);
				argplen = strlen(pspath) + 1;
				argpp = new char[argplen];
				strcpy_s(argpp, argplen, pspath);
				item = new DShaderResItem(argvp, argpp);
			}
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
	wchar_t* vs = CharToWCHAR(m_vsfile);
	wchar_t* ps = CharToWCHAR(m_psfile);
	DResObject* obj = DShader::Create(vs, ps);
	delete[] vs;
	delete[] ps;
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
