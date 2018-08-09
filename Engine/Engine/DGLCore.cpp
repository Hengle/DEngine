#include "DGLCore.h"

DGeometryRes::DGeometryRes(int vertexUsage, bool dynamic)
{
	m_isSupported = false;
	m_isInitialized = false;
	m_vertexUsage = vertexUsage;
	m_indexCount = 0;
	m_vertexCount = 0;

	m_isDynamic = dynamic;

	m_dataCount = 3;

	m_uv0Offset = -1;
	m_uv1Offset = -1;
	m_uv2Offset = -1;
	m_uv3Offset = -1;
	m_normalOffset = -1;
	m_colorOffset = -1;
	m_tangentOffset = -1;
	m_binormalOffset = -1;

	m_uv0Layout = -1;
	m_uv1Layout = -1;
	m_uv2Layout = -1;
	m_uv3Layout = -1;
	m_colorLayout = -1;
	m_normalLayout = -1;
	m_tangentLayout = -1;
	m_binormalLayout = -1;

	int layout = 1;

	if (vertexUsage & (1UL << DVertexUsage_NORMAL))
	{
		//m_dataSize += fsize * 3;
		m_normalOffset = m_dataCount;
		m_normalLayout = layout;
		m_dataCount += 3;
		layout += 1;
	}
	if (vertexUsage & (1UL << DVertexUsage_COLOR))
	{
		//m_dataSize += fsize * 4;
		m_colorOffset = m_dataCount;
		m_colorLayout = layout;
		m_dataCount += 4;
		layout += 1;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD0))
	{
		//m_dataSize += fsize * 2;
		m_uv0Offset = m_dataCount;
		m_uv0Layout = layout;
		m_dataCount += 2;
		layout += 1;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD1))
	{
		//m_dataSize += fsize * 2;
		m_uv1Offset = m_dataCount;
		m_uv1Layout = layout;
		m_dataCount += 2;
		layout += 1;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD2))
	{
		//m_dataSize += fsize * 2;
		m_uv2Offset = m_dataCount;
		m_uv2Layout = layout;
		m_dataCount += 2;
		layout += 1;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD3))
	{
		//m_dataSize += fsize * 2;
		m_uv3Offset = m_dataCount;
		m_uv3Layout = layout;
		m_dataCount += 2;
		layout += 1;
	}
	if (vertexUsage & (1UL << DVertexUsage_TANGENT))
	{
		//m_dataSize += fsize * 4;
		m_tangentOffset = m_dataCount;
		m_tangentLayout = layout;
		m_dataCount += 4;
		layout += 1;
	}
	if (vertexUsage & (1UL << DVertexUsage_BINORMAL))
	{
		//m_dataSize += fsize * 3;
		m_binormalOffset = m_dataCount;
		m_binormalLayout = layout;
		m_dataCount += 3;
		layout += 1;
	}

	m_dataSize = sizeof(float) * m_dataCount;
}

//void DMeshRes::Init(DMeshBufferDesc * desc)
//{
//	m_isInitialized = OnInit(desc);
//}

void DGeometryRes::Refresh(DGeometryBufferDesc * desc)
{
	if (desc == NULL)
		return;
	if (desc->indexCount <= 0 || desc->vertexCount <= 0)
		return;
	if (desc->vertices == nullptr)
		return;
	if (desc->indices == nullptr)
		return;
	m_indexCount = desc->indexCount;
	m_vertexCount = desc->vertexCount;


	float* vertices = new float[m_dataCount * desc->vertexCount];
	int i, j;
	for (i = 0; i < desc->vertexCount; i++)
	{
		j = 0;
		vertices[i*m_dataCount + j] = desc->vertices[i * 3];
		vertices[i*m_dataCount + j + 1] = desc->vertices[i * 3 + 1];
		vertices[i*m_dataCount + j + 2] = desc->vertices[i * 3 + 2];
		j += 3;
		if (m_normalOffset >= 0)
		{
			vertices[i*m_dataCount + j] = desc->normals != 0 ? desc->normals[i * 3] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->normals != 0 ? desc->normals[i * 3 + 1] : 0.0f;
			vertices[i*m_dataCount + j + 2] = desc->normals != 0 ? desc->normals[i * 3 + 2] : 0.0f;
			j += 3;
		}
		if (m_colorOffset >= 0)
		{
			vertices[i*m_dataCount + j] = desc->colors != 0 ? desc->colors[i * 4] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->colors != 0 ? desc->colors[i * 4 + 1] : 0.0f;
			vertices[i*m_dataCount + j + 2] = desc->colors != 0 ? desc->colors[i * 4 + 2] : 0.0f;
			vertices[i*m_dataCount + j + 3] = desc->colors != 0 ? desc->colors[i * 4 + 3] : 0.0f;
			j += 4;
		}
		if (m_uv0Offset >= 0)
		{
			vertices[i*m_dataCount + j] = desc->uvs != 0 ? desc->uvs[i * 2] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->uvs != 0 ? desc->uvs[i * 2 + 1] : 0.0f;
			j += 2;
		}
		if (m_tangentOffset >= 0)
		{
			vertices[i*m_dataCount + j] = desc->tangents != 0 ? desc->tangents[i * 4] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->tangents != 0 ? desc->tangents[i * 4 + 1] : 0.0f;
			vertices[i*m_dataCount + j + 2] = desc->tangents != 0 ? desc->tangents[i * 4 + 2] : 0.0f;
			vertices[i*m_dataCount + j + 3] = desc->tangents != 0 ? desc->tangents[i * 4 + 3] : 0.0f;
			j += 4;
		}
	}

	if (!m_isInitialized)
	{
		//m_isSupported = OnInit(desc); //OnInit(vertices, desc->indices, desc->vertexCount, desc->indexCount);
		m_isSupported = OnInit(vertices, desc->indices, desc->vertexCount, desc->indexCount);
		m_isInitialized = true;
	}
	else {
		if (m_isSupported)
			//OnRefresh(desc);
			OnRefresh(vertices, desc->indices, desc->vertexCount, desc->indexCount);
	}

	//delete[] vertices;
	//vertices = 0;
}

void DGeometryRes::Refresh(float * vertexbuffer, unsigned int * indexbuffer, int vertexCount, int indexCount)
{
	if (indexCount <= 0 || vertexCount <= 0)
		return;
	if (vertexbuffer == 0)
		return;
	if (indexbuffer == 0)
		return;
	m_indexCount = indexCount;
	m_vertexCount = vertexCount;

	if (!m_isInitialized)
	{
		m_isSupported = OnInit(vertexbuffer, indexbuffer, vertexCount, indexCount);
		m_isInitialized = true;
	}
	else {
		if (m_isSupported)
			OnRefresh(vertexbuffer, indexbuffer, vertexCount, indexCount);
	}
}

void DGeometryRes::DrawPrimitive(DGeometryTopology topology)
{
	if (m_isSupported)
		OnDraw(topology);
}

bool DGeometryRes::IsInitialized()
{
	return m_isInitialized;
}

DGLCore::DGLCore()
{
}


DGLCore::~DGLCore()
{
}

bool DGLCore::Init(int width, int height, bool)
{
	m_width = width;
	m_height = height;
	return true;
}

void DGLCore::GetResolution(float & width, float & height)
{
	width = m_width;
	height = m_height;
}