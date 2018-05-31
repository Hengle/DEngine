#pragma once
#include "DResObject.h"
#include <map>

enum DResType
{
	DRES_TYPE_TEXTURE = 0,
	DRES_TYPE_SHADER = 1,
	DRES_TYPE_MESH = 2,
	DRES_TYPE_MATERIAL = 3,
};

class DRes
{
private:
	class ResItem
	{
	public:
		ResItem();
		~ResItem();
		DResType GetResType();

	private:
		DResType m_resType;
	};

private:
	class ResGroup
	{
	public:
		ResGroup();
		~ResGroup();
		void Init();
	private:
		std::map<unsigned int, ResItem*> m_items;
	};

public:
	DRes();
	~DRes();
	void Init();
	void Shutdown();

	template<class T>
	static T Load(unsigned int groupid, unsigned int resid);
	static void UnLoad(unsigned int groupid, unsigned int resid);
	static void LoadGroup(unsigned int groupid);
	static void UnLoadGroup(unsigned int groupid);
	static void UnLoadAll();
	static void HasGroup(unsigned int groupid);
	static void HasRes(unsigned int groupid, unsigned int resid);

private:
	void LoadResList();

private:
	std::map<unsigned int, ResGroup*>* m_groups;
};

template<class T>
inline T DRes::Load(unsigned int groupid, unsigned int resid)
{
	return T();
}
