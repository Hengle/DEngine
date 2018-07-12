#pragma once
#include "DObject.h"

/*资源类对象-该类的子类全部可以通过资源管理进行加载卸载*/
class DResObject : public DObject
{
public:
	DResObject();
	~DResObject();

	virtual void Destroy() = 0;
};

