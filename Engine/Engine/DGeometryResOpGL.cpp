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
	glDeleteBuffers(1, &m_vertexBuffer);
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

	/*if (m_normalOffset >= 0)
	{
		glGenBuffers(1, &m_normalsBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc->vertexCount * 3, desc->normals, GL_STATIC_DRAW);
	}*/

	/*if (m_normalOffset >= 0)
	{
		glGenBuffers(1, &m_normalsBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc->vertexCount * 3, desc->normals, GL_STATIC_DRAW);
	}*/

	return false;
}

void DGeometryResOpGL::OnDraw(DGeometryTopology)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 18); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}

#endif