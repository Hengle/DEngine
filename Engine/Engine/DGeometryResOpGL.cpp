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
	/*static const GLfloat g_vertex_buffer_data[] = {

		0.5,-0.5,0.5,
		0.5,0.5,0.5,
		-0.5,0.5,0.5,
		0.5,-0.5,0.5,
		-0.5,0.5,0.5,
		-0.5,-0.5,0.5,
		0.5,0.5,0.5,
		0.5,0.5,-0.5,
		-0.5,0.5,-0.5,
		0.5,0.5,0.5,
		-0.5,0.5,-0.5,
		-0.5,0.5,0.5,
		0.5,0.5,-0.5,
		0.5,-0.5,-0.5,
		-0.5,-0.5,-0.5,
		0.5,0.5,-0.5,
		-0.5,-0.5,-0.5,
		-0.5,0.5,-0.5,
		0.5,-0.5,-0.5,
		0.5,-0.5,0.5,
		-0.5,-0.5,0.5,
		0.5,-0.5,-0.5,
		-0.5,-0.5,0.5,
		-0.5,-0.5,-0.5,
		-0.5,-0.5,0.5,
		-0.5,0.5,0.5,
		-0.5,0.5,-0.5,
		-0.5,-0.5,0.5,
		-0.5,0.5,-0.5,
		-0.5,-0.5,-0.5,
		0.5,-0.5,-0.5,
		0.5,0.5,-0.5,
		0.5,0.5,0.5,
		0.5,-0.5,-0.5,
		0.5,0.5,0.5,
		0.5,-0.5,0.5,
	};*/

	/*static const GLfloat g_vertex_buffer_data[]
	{
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
	};*/

	/*static const unsigned int g_index_buffer[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
		18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
	};*/

	/*static const unsigned int g_index_buffer[] = {
		0,1,2
	};*/

	glGenVertexArrays(1, &m_vertexArrayId);
	glBindVertexArray(m_vertexArrayId);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertexCount*3, desc->vertices, GL_STATIC_DRAW);

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

	unsigned int* indexes = new unsigned int[m_indexCount];
	int i = 0;
	for (i = 0; i < m_indexCount; i++)
	{
		indexes[i] = desc->indices[i];
	}

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indexCount, indexes, GL_STATIC_DRAW);


	delete[] indexes;

	return true;
}

void DGeometryResOpGL::OnDraw(DGeometryTopology)
{
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	//glVertexAttribPointer(
	//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride
	//	(void*)0            // array buffer offset
	//	);
	// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	// Draw the triangles !
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(
		GL_TRIANGLES,      // mode
		size / sizeof(unsigned int),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
		);
	//glFlush();
}

#endif