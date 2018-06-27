#pragma once
#include "DGLCore.h"

static class DModelLoader
{
public:
	static bool LoadObj(const char *, DMeshBufferDesc*);

	static bool CreateCube(DMeshBufferDesc*);

	static bool CreatePlane(DMeshBufferDesc*);
};

