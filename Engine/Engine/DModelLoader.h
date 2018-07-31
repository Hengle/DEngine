#pragma once
#include "DGLCore.h"

static class DModelLoader
{
public:
	static bool LoadObj(const char *, DGeometryBufferDesc*);

	static bool CreateCube(DGeometryBufferDesc*);

	static bool CreatePlane(DGeometryBufferDesc*);

	static bool CreateSphere(DGeometryBufferDesc*);
};

