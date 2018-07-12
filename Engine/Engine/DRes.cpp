#include "DRes.h"
#include "DSystem.h"

DRes::DRes()
{
}

DRes::~DRes()
{
}

void DRes::Init(char* resmanifest)
{
	m_groups = new std::map<unsigned int, DResGroup*>();
	LoadResManifest(resmanifest);
}

void DRes::Shutdown()
{
	if (m_groups != NULL)
	{
		std::map<unsigned int, DResGroup*>::iterator iter;
		for (iter = m_groups->begin(); iter != m_groups->end(); iter++)
		{
			iter->second->Release();
			delete iter->second;
		}
		m_groups->clear();
		m_groups = NULL;
	}
}

DResObject* DRes::LoadRes(unsigned int groupid, unsigned int resid)
{
	DRes* instance = DSystem::GetResMgr();
	if (instance == NULL)
		return NULL;
	if (instance->m_groups == NULL)
		return NULL;
	if (instance->m_groups->find(groupid) != instance->m_groups->end())
	{
		DResGroup* group = instance->m_groups->at(groupid);
		if (group != NULL)
		{
			return group->LoadRes(resid);
		}
	}
	return NULL;
}

void DRes::UnLoad(unsigned int groupid, unsigned int resid)
{
	DRes* instance = DSystem::GetResMgr();
	if (instance == NULL)
		return;
	if (instance->m_groups == NULL)
		return;
	if (instance->m_groups->find(groupid) != instance->m_groups->end())
	{
		DResGroup* group = instance->m_groups->at(groupid);
		if (group != NULL)
		{
			group->UnLoadRes(resid);
		}
	}
}

void DRes::LoadGroup(unsigned int groupid)
{
	DRes* instance = DSystem::GetResMgr();
	if (instance == NULL)
		return;
	if (instance->m_groups == NULL)
		return;
	if (instance->m_groups->find(groupid) != instance->m_groups->end())
	{
		DResGroup* group = instance->m_groups->at(groupid);
		if (group != NULL)
		{
			group->LoadAll();
		}
	}
}

void DRes::UnLoadGroup(unsigned int groupid)
{
	DRes* instance = DSystem::GetResMgr();
	if (instance == NULL)
		return;
	if (instance->m_groups == NULL)
		return;
	if (instance->m_groups->find(groupid) != instance->m_groups->end())
	{
		DResGroup* group = instance->m_groups->at(groupid);
		if (group != NULL)
		{
			group->UnLoadAll();
		}
	}
}

void DRes::UnLoadAll()
{
	DRes* instance = DSystem::GetResMgr();
	if (instance == NULL)
		return;
	if (instance->m_groups == NULL)
		return;
	std::map<unsigned int, DResGroup*>::iterator iter;
	for (iter = instance->m_groups->begin(); iter != instance->m_groups->end(); iter++)
	{
		iter->second->UnLoadAll();
	}
}

bool DRes::HasGroup(unsigned int groupid)
{
	DRes* instance = DSystem::GetResMgr();
	if (instance == NULL)
		return false;
	if (instance->m_groups == NULL)
		return false;
	if (instance->m_groups->find(groupid) != instance->m_groups->end())
		return true;
	return false;
}

bool DRes::HasRes(unsigned int groupid, unsigned int resid)
{
	DRes* instance = DSystem::GetResMgr();
	if (instance == NULL)
		return false;
	if (instance->m_groups == NULL)
		return false;
	if (instance->m_groups->find(groupid) != instance->m_groups->end())
	{
		DResGroup* group = instance->m_groups->at(groupid);
		if (group != NULL)
			return group->HasRes(resid);
	}
	return false;
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
	ifile.close();
}

void DRes::LoadResGroupManifest(ifstream & ifile, unsigned int groupid)
{
	unsigned int rid;
	char rdef[64], rtype[64];

	DResGroup* group = new DResGroup();
	m_groups->insert(std::pair<unsigned int, DResGroup*>(groupid, group));

	DResItem* item = NULL;

	while (!ifile.eof())
	{
		ifile >> rdef;
		if (strcmp(rdef, "#RES_BEGIN") == 0)
		{
			ifile >> rtype;
			ifile >> rid;

			if (strcmp(rtype, "SHADER") == 0)
			{
				item = DShaderResItem::LoadManifest(ifile);
				if (item != NULL)
					group->AddItem(rid, item);
			}
			else if (strcmp(rtype, "TEXTURE2D") == 0)
			{
				item = DTexture2DResItem::LoadManifest(ifile);
				if (item != NULL)
					group->AddItem(rid, item);
			}
			else if (strcmp(rtype, "MATERIAL") == 0)
			{
				item = DMaterialResItem::LoadManifest(ifile);
				if (item != NULL)
					group->AddItem(rid, item);
			}
			else if (strcmp(rtype, "GEOMETRY") == 0)
			{
				item = DGeometryResItem::LoadManifest(ifile);
				if (item != NULL)
					group->AddItem(rid, item);
			}
		}
		if (strcmp(rdef, "#RES_GROUP_END") == 0)
		{
			return;
		}
	}
}
