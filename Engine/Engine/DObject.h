#pragma once
#include <d3d11.h>

class DObject
{
public:
	DObject();
	~DObject();
	virtual void Destroy() = 0;
};

