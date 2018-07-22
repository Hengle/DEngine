#pragma once
#include "DMath.h"

class DCuller
{
public:
	DCuller();
	~DCuller();

	static bool Culling(DBounds bounds);

private:
	static int ComputeOutCode(DVector4&, DMatrix4x4& projection);
};

