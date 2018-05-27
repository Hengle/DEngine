#pragma once

class DObject
{
public:
	DObject();
	~DObject();
	virtual void Destroy() = 0;
};

