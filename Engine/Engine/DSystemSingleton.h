#pragma once
template<class T>
class DSystemSingleton
{
public:
	DSystemSingleton();
	~DSystemSingleton();
	
	static void ReleaseInstance();

protected:
	static T* GetInstance();

private:
	static T* sInstance;
};

