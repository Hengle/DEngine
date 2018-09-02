#pragma once
#include "DGLCore.h"

static class DModelLoader
{
public:
	static bool LoadObj(const char *, DGeometryBufferDesc*);

	static bool LoadMesh(const char *, DGeometryBufferDesc*, bool& dynamic);

private:
	static void CalculateTangent(DVector3*, int, int, int, DGeometryBufferDesc*);
};

