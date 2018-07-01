#include "DResGroup.h"


DResGroup::DResGroup()
{
}


DResGroup::~DResGroup()
{
}

void DResGroup::Release()
{
	std::map<unsigned int, DResItem*>::iterator iter;
	for (iter = m_items.begin(); iter != m_items.end(); iter++)
	{
		iter->second->Release();
		delete iter->second;
	}
	m_items.clear();
}

void DResGroup::UnLoadAll()
{
	std::map<unsigned int, DResItem*>::iterator iter;
	for (iter = m_items.begin(); iter != m_items.end(); iter++)
	{
		if (iter->second != NULL)
			iter->second->Unload();
	}
}

void DResGroup::LoadAll()
{
	std::map<unsigned int, DResItem*>::iterator iter;
	for (iter = m_items.begin(); iter != m_items.end(); iter++)
	{
		if (iter->second != NULL)
			iter->second->Load();
	}
}

DResObject * DResGroup::LoadRes(unsigned int resid)
{
	if (m_items.find(resid) != m_items.end())
	{
		DResItem* item = m_items.at(resid);
		if (item != NULL)
		{
			item->Load();
			return item->GetRes();
		}
	}
	return NULL;
}

void DResGroup::UnLoadRes(unsigned int resid)
{
	if (m_items.find(resid) != m_items.end())
	{
		DResItem* item = m_items.at(resid);
		if (item != NULL)
			item->Unload();
	}
}

bool DResGroup::HasRes(unsigned int resid)
{
	if (m_items.find(resid) != m_items.end())
		return true;
	return false;
}

void DResGroup::AddItem(unsigned int resid, DResItem * item)
{
	if (m_items.find(resid) == m_items.end())
	{
		m_items.insert(std::pair<unsigned int, DResItem*>(resid, item));
	}
}
