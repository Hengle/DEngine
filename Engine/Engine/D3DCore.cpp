#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11

#include "D3DCore.h"

unsigned int RasterizerState::GetKey()
{
	unsigned int key = 0;
	key = fillMode << 2;
	key = key | cullMode;
	return key;
}

unsigned long DepthStencilState::GetKey()
{
	unsigned long key = 0;
	if (zwrite)
		key = 1L << 31;
	unsigned long func = ztest << 28;
	key = key | func;
	if (enableStencil)
	{
		key = key | (stencilWriteMask << 20);
		key = key | (stencilReadMask << 12);
		key = key | (stencilComp << 9);
		key = key | (stencilPassOp << 6);
		key = key | (stencilFailOp << 3);
		key = key | (stencilZFailOp);
	}

	return key;
}

unsigned long BlendState::GetKey()
{
	unsigned long key = 0;
	key = blendOp << 8;
	key = key | (srcfactor << 4);
	key = key | dstfactor;
	return key;
}

DGeometryResD3D::DGeometryResD3D(int vertexUsage, bool dynamic) : DGeometryRes(vertexUsage, dynamic)
{
}

void DGeometryResD3D::OnRefresh(DGeometryBufferDesc * desc)
{
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
	}

	OnRefresh(vertices, desc->indices, desc->vertexCount, desc->indexCount);

	delete[] vertices;
	vertices = 0;
}

bool DGeometryResD3D::OnInit(DGeometryBufferDesc * desc)
{
	if (desc == NULL)
		return false;
	if (desc->indexCount <= 0 || desc->vertexCount <= 0)
		return false;

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
	}

	if (!OnInit(vertices, desc->indices, desc->vertexCount, desc->indexCount))
		return false;

	delete[] vertices;
	vertices = 0;

	return true;
}

#endif