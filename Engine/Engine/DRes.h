#pragma once
#include "DResObject.h"
#include "DResGroup.h"
#include <map>
#include <fstream>

using namespace std;

class DRes
{

public:
	DRes();
	~DRes();
	void Init(char*);
	void Shutdown();

	template<class T>
	static T* Load(unsigned int groupid, unsigned int resid);
	static void UnLoad(unsigned int groupid, unsigned int resid);
	static void LoadGroup(unsigned int groupid);
	static void UnLoadGroup(unsigned int groupid);
	static void UnLoadAll();
	static bool HasGroup(unsigned int groupid);
	static bool HasRes(unsigned int groupid, unsigned int resid);

private:
	static DResObject* LoadRes(unsigned int groupid, unsigned int resid);
	bool LoadResManifest(char * fileName);
	void LoadResGroupManifest(ifstream&, unsigned int groupid);

private:
	std::map<unsigned int, DResGroup*>* m_groups;
};

template<class T>
inline T * DRes::Load(unsigned int groupid, unsigned int resid)
{
	return (T*)LoadRes(groupid, resid);
}
