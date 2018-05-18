#include "DSystemSingleton.h"

template<class T>
DSystemSingleton<T>::DSystemSingleton()
{
}

template<class T>
DSystemSingleton<T>::~DSystemSingleton()
{
}

template<class T>
void DSystemSingleton<T>::ReleaseInstance()
{
	if (sInstance != NULL) {
		delete sInstance;
	}
	sInstance = NULL;
}

template<class T>
T* DSystemSingleton<T>::GetInstance()
{
	if (sInstance == NULL) {
		sInstance = new T();
	}
	return sInstance;
}
