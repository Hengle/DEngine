#pragma once
#include "DResObject.h"
#include "DResGroup.h"
#include <map>
#include <fstream>

using namespace std;

/*资源管理器*/
class DRes
{

public:
	DRes();
	~DRes();
	/*初始化*/
	void Init(char* /*资源清单路径*/);
	void Shutdown();

	template<class T>
	/*加载资源*/
	static T* Load(unsigned int groupid /*资源组id*/, unsigned int resid /*资源id*/);
	/*卸载资源*/
	static void UnLoad(unsigned int groupid /*资源组id*/, unsigned int resid /*资源id*/);
	/*加载资源组*/
	static void LoadGroup(unsigned int groupid /*资源组id*/);
	/*卸载资源组*/
	static void UnLoadGroup(unsigned int groupid /*资源组id*/);
	/*卸载全部*/
	static void UnLoadAll();
	/*是否包含资源组*/
	static bool HasGroup(unsigned int groupid /*资源组id*/);
	/*是否包含资源*/
	static bool HasRes(unsigned int groupid /*资源组id*/, unsigned int resid /*资源id*/);

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
