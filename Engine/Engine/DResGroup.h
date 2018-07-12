#pragma once
#include "DResItem.h"
#include <map>

/*资源组*/
class DResGroup
{
public:
	DResGroup();
	~DResGroup();
	void Release();
	void UnLoadAll();
	void LoadAll();
	DResObject* LoadRes(unsigned int);
	void UnLoadRes(unsigned int);
	bool HasRes(unsigned int);
	void AddItem(unsigned int, DResItem*);
private:
	std::map<unsigned int, DResItem*> m_items;
};

