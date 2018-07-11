#pragma once
#include "DObject.h"

class DResObject : public DObject
{
public:
	DResObject();
	~DResObject();

	virtual void Destroy() = 0;
};

