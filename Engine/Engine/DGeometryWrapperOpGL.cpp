#ifdef _DGAPI_OPENGL

#include "DGeometryWrapperOpGL.h"

DGeometryWrapperOpGL::DGeometryWrapperOpGL(int vertexUsage, bool dynamic) : DGeometryWrapper(vertexUsage, dynamic)
{
	m_vertexArrayId = 0xffffff;
	m_vertexBuffer = 0xffffff;

	m_indexBuffer = 0xffffff;
}

DGeometryWrapperOpGL::~DGeometryWrapperOpGL()
{
}

void DGeometryWrapperOpGL::Release()
{
	if (m_vertexArrayId != 0xffffff)
		glDeleteVertexArrays(1, &m_vertexArrayId);
	if (m_vertexBuffer != 0xffffff)
		glDeleteBuffers(1, &m_vertexBuffer);
	if (m_indexBuffer != 0xffffff)
		glDeleteBuffers(1, &m_indexBuffer);
}

void DGeometryWrapperOpGL::OnRefresh(float * vertexbuffer, unsigned int * indexbuffer, int vertexCount, int indexCount)
{
	glBindVertexArray(m_vertexArrayId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, vertexCount*m_dataSize, vertexbuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexCount, indexbuffer, GL_STATIC_DRAW);

}

bool DGeometryWrapperOpGL::OnInit(float * vertexbuffer, unsigned int * indexbuffer, int vertexCount, int indexCount)
{
	int offset = 0;

	glGenVertexArrays(1, &m_vertexArrayId);
	glGenBuffers(1, &m_vertexBuffer);
	glGenBuffers(1, &m_indexBuffer);
	
	OnRefresh(vertexbuffer, indexbuffer, vertexCount, indexCount);

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

	if (m_normalOffset >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		glVertexAttribPointer(
			m_normalLayout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			m_dataSize,                  // stride
			(unsigned char*)NULL + (offset * sizeof(float))           // array buffer offset
			);
		offset += 3;
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
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		glVertexAttribPointer(
			m_uv0Layout,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			m_dataSize,                  // stride
			(unsigned char*)NULL + (offset * sizeof(float))          // array buffer offset
			);
		offset += 2;
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

	return true;
}

void DGeometryWrapperOpGL::OnDraw(DGeometryTopology topology)
{
	GLenum mode;
	switch (topology)
	{
	case DGeometryTopology_LineList:
		mode = GL_LINES;
		break;
	case DGeometryTopology_LineStrip:
		mode = GL_LINE_STRIP;
		break;
	case DGeometryTopology_PointList:
		mode = GL_POINTS;
		break;
	case DGeometryTopology_TriangleList:
		mode = GL_TRIANGLES;
		break;
	case DGeometryTopology_TriangleStrip:
		mode = GL_TRIANGLE_STRIP;
		break;
	default:
		mode = GL_TRIANGLES;
		break;
	}
	glBindVertexArray(m_vertexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(
		mode,      // mode
		size / sizeof(unsigned int),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
		);
}

#endif