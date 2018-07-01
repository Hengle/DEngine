#include "DRes.h"

DRes::DRes()
{
}

DRes::~DRes()
{
}

void DRes::Init()
{
	m_groups = new std::map<unsigned int, ResGroup*>();
	//LoadResManifest("../Res/ResManifest.dres");
}

void DRes::Shutdown()
{
}

void DRes::UnLoad(unsigned int groupid, unsigned int resid)
{
}

void DRes::LoadGroup(unsigned int groupid)
{
}

void DRes::UnLoadGroup(unsigned int groupid)
{
}

void DRes::UnLoadAll()
{
}

void DRes::HasGroup(unsigned int groupid)
{
}

void DRes::HasRes(unsigned int groupid, unsigned int resid)
{
}

bool DRes::LoadResManifest(char * fileName)
{
	ifstream ifile;
	ifile.open(fileName);
	if (ifile.fail())
	{
		return nullptr;
	}

	char rdef[64];
	unsigned int groupid;
	int comp;

	while (!ifile.eof())
	{
		ifile >> rdef;
		comp = strcmp(rdef, "#RES_GROUP_BEGIN");
		if (comp == 0)
		{
			ifile >> groupid;
			LoadResGroupManifest(ifile, groupid);
		}
	}
}

void DRes::LoadResGroupManifest(ifstream & ifile, unsigned int groupid)
{
	unsigned int rid;
	char rdef[64], rtype[64];

	while (!ifile.eof())
	{
		ifile >> rdef;
		if (strcmp(rdef, "#RES_BEGIN") == 0)
		{
			ifile >> rtype;
			ifile >> rid;

			if (strcmp(rtype, "SHADER") == 0)
			{
				LoadShaderResManifest(ifile, rid);
			}
		}
	}
}

void DRes::LoadShaderResManifest(ifstream & iflie, unsigned int resid)
{
	char gapi[32], vspath[512], pspath[512];
	iflie >> gapi >> vspath >> pspath;
}

DRes::ResGroup::ResGroup()
{
}

DRes::ResGroup::~ResGroup()
{
}

void DRes::ResGroup::Init()
{
}

DRes::ResItem::ResItem()
{
}

DRes::ResItem::~ResItem()
{
}

DResType DRes::ResItem::GetResType()
{
	return DResType();
}
