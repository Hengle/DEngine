#ifdef _DGAPI_OPENGL

#include "DGeometryResOpGL.h"

DGeometryResOpGL::DGeometryResOpGL(int vertexUsage, bool dynamic) : DGeometryRes(vertexUsage, dynamic)
{
	m_vertexArrayId = 0xffffff;
	m_vertexBuffer = 0xffffff;
	/*m_colorBuffer = 0xffffff;
	m_normalsBuffer = 0xffffff;
	m_uv0Buffer = 0xffffff;*/

	m_indexBuffer = 0xffffff;
}

DGeometryResOpGL::~DGeometryResOpGL()
{
}

void DGeometryResOpGL::Release()
{
	if (m_vertexArrayId != 0xffffff)
		glDeleteVertexArrays(1, &m_vertexArrayId);
	if (m_vertexBuffer != 0xffffff)
		glDeleteBuffers(1, &m_vertexBuffer);
	/*if (m_colorBuffer != 0xffffff)
		glDeleteBuffers(1, &m_colorBuffer);
	if (m_normalsBuffer != 0xffffff)
		glDeleteBuffers(1, &m_normalsBuffer);
	if (m_uv0Buffer != 0xffffff)
		glDeleteBuffers(1, &m_uv0Buffer);*/
	if (m_indexBuffer != 0xffffff)
		glDeleteBuffers(1, &m_indexBuffer);
}

//void DGeometryResOpGL::OnRefresh(DGeometryBufferDesc * desc)
//{
//}

//bool DGeometryResOpGL::OnInit(DGeometryBufferDesc * desc)
//{
//
//	glGenVertexArrays(1, &m_vertexArrayId);
//	glBindVertexArray(m_vertexArrayId);
//
//	glGenBuffers(1, &m_vertexBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc ->vertexCount*3, desc->vertices, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
//	glVertexAttribPointer(
//		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//		3,                  // size
//		GL_FLOAT,           // type
//		GL_FALSE,           // normalized?
//		0,                  // stride
//		(void*)0            // array buffer offset
//		);
//
//	//glDisableVertexAttribArray(0);
//
//	if (m_normalOffset >= 0)
//	{
//		glGenBuffers(1, &m_normalsBuffer);
//		glBindBuffer(GL_ARRAY_BUFFER, m_normalsBuffer);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc->vertexCount * 3, desc->normals, GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(m_normalLayout);
//
//		glBindBuffer(GL_ARRAY_BUFFER, m_normalsBuffer);
//		glVertexAttribPointer(
//			m_normalLayout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//			3,                  // size
//			GL_FLOAT,           // type
//			GL_FALSE,           // normalized?
//			0,                  // stride
//			(void*)0            // array buffer offset
//			);
//
//		//glDisableVertexAttribArray(m_normalLayout);
//	}
//	if (m_uv0Offset >= 0)
//	{
//		glGenBuffers(1, &m_uv0Buffer);
//		glBindBuffer(GL_ARRAY_BUFFER, m_uv0Buffer);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc->vertexCount * 2, desc->uvs, GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(m_uv0Layout);
//
//		glBindBuffer(GL_ARRAY_BUFFER, m_uv0Buffer);
//		glVertexAttribPointer(
//			m_uv0Layout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//			2,                  // size
//			GL_FLOAT,           // type
//			GL_FALSE,           // normalized?
//			0,                  // stride
//			(void*)0            // array buffer offset
//			);
//
//		//glDisableVertexAttribArray(m_uv0Layout);
//	}
//
//	unsigned int* indexes = new unsigned int[m_indexCount];
//	int i = 0;
//	for (i = 0; i < m_indexCount; i++)
//	{
//		indexes[i] = desc->indices[i];
//	}
//
//	glGenBuffers(1, &m_indexBuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*desc->indexCount, indexes, GL_STATIC_DRAW);
//
//
//	delete[] indexes;
//
//	return true;
//}

void DGeometryResOpGL::OnRefresh(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
}

bool DGeometryResOpGL::OnInit(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	int offset = 0;

	glGenVertexArrays(1, &m_vertexArrayId);
	glBindVertexArray(m_vertexArrayId);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount*m_dataSize, vertexbuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	if (m_normalLayout >= 0)
	{
		glEnableVertexAttribArray(m_normalLayout);
	}
	if (m_colorLayout >= 0)
	{
		glEnableVertexAttribArray(m_colorLayout);
	}
	if (m_uv0Layout >= 0)
	{
		glEnableVertexAttribArray(m_uv0Layout);
	}
	if (m_tangentLayout >= 0)
	{
		glEnableVertexAttribArray(m_tangentLayout);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		m_dataSize,                  // stride
		(void*)0            // array buffer offset
		);

	offset += 3;

	//glDisableVertexAttribArray(0);

	if (m_normalOffset >= 0)
	{
		//glGenBuffers(1, &m_normalsBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc->vertexCount * 3, desc->normals, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(m_normalLayout);

		//glBindBuffer(GL_ARRAY_BUFFER, m_normalsBuffer);
		glVertexAttribPointer(
			m_normalLayout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			m_dataSize,                  // stride
			(unsigned char*)NULL + (offset * sizeof(float))           // array buffer offset
			);
		offset += 3;
		//glDisableVertexAttribArray(m_normalLayout);
	}
	if (m_colorOffset >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glVertexAttribPointer(
			m_colorLayout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			m_dataSize,                  // stride
			(unsigned char*)NULL + (offset * sizeof(float))           // array buffer offset
			);
		offset += 4;
	}
	if (m_uv0Offset >= 0)
	{
		//glGenBuffers(1, &m_uv0Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*desc->vertexCount * 2, desc->uvs, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(m_uv0Layout);

		//glBindBuffer(GL_ARRAY_BUFFER, m_uv0Buffer);
		glVertexAttribPointer(
			m_uv0Layout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			m_dataSize,                  // stride
			(unsigned char*)NULL + (offset * sizeof(float))          // array buffer offset
			);
		offset += 2;
		//glDisableVertexAttribArray(m_uv0Layout);
	}
	if (m_tangentOffset >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glVertexAttribPointer(
			m_tangentLayout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			m_dataSize,                  // stride
			(unsigned char*)NULL + (offset * sizeof(float))           // array buffer offset
			);
		offset += 4;
	}

	unsigned int* indexes = new unsigned int[m_indexCount];
	int i = 0;
	for (i = 0; i < m_indexCount; i++)
	{
		indexes[i] = indexbuffer[i];
	}

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexCount, indexes, GL_STATIC_DRAW);


	delete[] indexes;

	return true;
}

void DGeometryResOpGL::OnDraw(DGeometryTopology)
{
	glBindVertexArray(m_vertexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(
		GL_TRIANGLES,      // mode
		size / sizeof(unsigned int),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
		);
}

#endif