#pragma once
template<class T>
class DSystemSingleton
{
public:
	DSystemSingleton(const DSystemSingleton& src) {}
	DSystemSingleton &operator=(const DSystemSingleton& src) {};
	
	static void CreateInstance(T** obj)
	{
		if (sInstance == NULL) {
			sInstance = new T();
		}
		(*obj) = sInstance;
	}

protected:
	DSystemSingleton(){}
	~DSystemSingleton(){}

	static T* GetInstance()
	{
		if (sInstance == NULL) {
			sInstance = new T();
		}
		return sInstance;
	}

private:
	static T* sInstance;

	class Garbo
	{
	public:
		Garbo()
		{
			if (DSystemSingleton::sInstance)
			{
				delete DSystemSingleton::sInstance;
				DSystemSingleton::sInstance = NULL;
			}
		}
	};
	static Garbo garbo;
};

template <class T>
T* DSystemSingleton<T>::sInstance = NULL;