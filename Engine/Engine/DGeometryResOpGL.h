#pragma once
#ifdef _DGAPI_OPENGL

#include "DOpenGLCore.h"

/*
Geometry资源OpenGL底层
*/
class DGeometryResOpGL : public DGeometryRes
{
public:
	DGeometryResOpGL(int vertexUsage /*顶点用法描述*/, bool dynamic /*是否为动态mesh*/);
	~DGeometryResOpGL();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);
};

#endif