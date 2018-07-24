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
	virtual void OnRefresh(DGeometryBufferDesc* desc);
	virtual bool OnInit(DGeometryBufferDesc* desc);
	virtual void OnDraw(DGeometryTopology);

private:
	GLuint m_vertexArrayId;
	GLuint m_vertexBuffer;
	GLuint m_colorBuffer;
	GLuint m_normalsBuffer;
	GLuint m_uv0Buffer;
};

#endif