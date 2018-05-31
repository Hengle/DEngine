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
	LoadResList();
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

void DRes::LoadResList()
{
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
