#ifdef _DGAPI_OPENGL

#include "DGeometryResOpGL.h"

DGeometryResOpGL::DGeometryResOpGL(int vertexUsage, bool dynamic) : DGeometryRes(vertexUsage, dynamic)
{
}

DGeometryResOpGL::~DGeometryResOpGL()
{
}

void DGeometryResOpGL::Release()
{
}

void DGeometryResOpGL::OnRefresh(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
}

bool DGeometryResOpGL::OnInit(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	return false;
}

void DGeometryResOpGL::OnDraw(DGeometryTopology)
{
}

#endif