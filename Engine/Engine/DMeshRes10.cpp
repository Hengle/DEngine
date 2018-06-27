#include "DMeshRes10.h"

DMeshRes10::DMeshRes10(ID3D10Device * device, int vertexUsage) : DMeshRes(vertexUsage)
{
	m_device = device;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_dataSize = 0;
	m_dataCount = 0;
	m_indexCount = 0;

	float fsize = sizeof(float);
	m_dataSize = fsize * 3;
	m_dataCount = 3;
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD0))
	{
		m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD1))
	{
		m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV1 = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD2))
	{
		m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV2 = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD3))
	{
		m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV3 = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_NORMAL))
	{
		m_dataSize += fsize * 3;
		m_dataCount += 3;
		m_hasNormal = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_COLOR))
	{
		m_dataSize += fsize * 4;
		m_dataCount += 4;
		m_hasColor = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TANGENT))
	{
		m_dataSize += fsize * 4;
		m_dataCount += 4;
		m_hasTangent = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_BINORMAL))
	{
		m_dataSize += fsize * 3;
		m_dataCount += 3;
		m_hasBinormal = true;
	}
}

DMeshRes10::~DMeshRes10()
{
}

void DMeshRes10::Release()
{
	if (m_vertexBuffer != NULL)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
	if (m_indexBuffer != NULL)
	{
		m_indexBuffer->Release();
		m_indexBuffer = NULL;
	}
	m_device = NULL;
}

bool DMeshRes10::OnInit(DMeshBufferDesc * desc)
{
	if (desc == NULL)
		return false;
	if (desc->indexCount <= 0 || desc->vertexCount <= 0 || m_dataSize <= 0)
		return false;
	if (desc->vertices == nullptr)
		return false;
	if (desc->indices == nullptr)
		return false;
	D3D10_BUFFER_DESC  vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//m_dataSize = desc->dataSize;
	m_indexCount = desc->indexCount;

	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_dataSize * desc->vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	float* vertices = new float[m_dataCount * desc->vertexCount];
	int i, j;
	for (i = 0; i < desc->vertexCount; i++)
	{
		j = 0;
		vertices[i*m_dataCount + j] = desc->vertices[i * 3];
		vertices[i*m_dataCount + j + 1] = desc->vertices[i * 3 + 1];
		vertices[i*m_dataCount + j + 2] = desc->vertices[i * 3 + 2];
		j += 3;
		if (m_hasNormal)
		{
			vertices[i*m_dataCount + j] = desc->normals != 0 ? desc->normals[i * 3] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->normals != 0 ? desc->normals[i * 3 + 1] : 0.0f;
			vertices[i*m_dataCount + j + 2] = desc->normals != 0 ? desc->normals[i * 3 + 2] : 0.0f;
			j += 3;
		}
		if (m_hasColor)
		{
			vertices[i*m_dataCount + j] = desc->colors != 0 ? desc->colors[i * 4] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->colors != 0 ? desc->colors[i * 4 + 1] : 0.0f;
			vertices[i*m_dataCount + j + 2] = desc->colors != 0 ? desc->colors[i * 4 + 2] : 0.0f;
			vertices[i*m_dataCount + j + 3] = desc->colors != 0 ? desc->colors[i * 4 + 3] : 0.0f;
			j += 4;
		}
		if (m_hasUV)
		{
			vertices[i*m_dataCount + j] = desc->uvs != 0 ? desc->uvs[i * 2] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->uvs != 0 ? desc->uvs[i * 2 + 1] : 0.0f;
			j += 2;
		}
	}

	vertexData.pSysMem = vertices;

	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		delete[] vertices;
		return false;
	}

	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * desc->indexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	indexData.pSysMem = desc->indices;

	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		delete[] vertices;
		return false;
	}
	delete[] vertices;
	return true;
}

void DMeshRes10::OnRefresh(DMeshBufferDesc *)
{
}

void DMeshRes10::OnDraw(DMeshTopology topology)
{
	unsigned int stride;
	unsigned int offset;


	stride = m_dataSize;
	offset = 0;

	ID3D10Buffer* vbuffer = m_vertexBuffer;
	ID3D10Buffer* ibuffer = m_indexBuffer;

	m_device->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_device->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	if (topology == DMeshTopology_LineList)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	else if (topology == DMeshTopology_LineStrip)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	else if (topology == DMeshTopology_PointList)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	else if (topology == DMeshTopology_TriangleList)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (topology == DMeshTopology_TriangleStrip)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_device->DrawIndexed(m_indexCount, 0, 0);
}
