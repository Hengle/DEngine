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
	glDeleteVertexArrays(1, &m_vertexArrayId);
	
}

void DGeometryResOpGL::OnRefresh(DGeometryBufferDesc * desc)
{
}

bool DGeometryResOpGL::OnInit(DGeometryBufferDesc * desc)
{
	glGenVertexArrays(1, &m_vertexArrayId);
	glBindVertexArray(m_vertexArrayId);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc->vertexCount * 3, desc->vertices, GL_STATIC_DRAW);

	if (m_normalOffset >= 0)
	{
		
	}

	return false;
}

void DGeometryResOpGL::OnDraw(DGeometryTopology)
{
}

#endif