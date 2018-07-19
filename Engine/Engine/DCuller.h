#pragma once
#include "DMath.h"

class DCuller
{
public:
	DCuller();
	~DCuller();

	bool Culling(DBounds bounds);
};

