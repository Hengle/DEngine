#include "DMeshRes10.h"

DMeshRes10::DMeshRes10(ID3D10Device * device, int vertexUsage, bool dynamic) : DMeshRes(vertexUsage, dynamic)
{
	m_device = device;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	//m_dataSize = 0;
	//m_dataCount = 0;
	//m_indexCount = 0;

	//m_dataSize = sizeof(float) * m_dataCount;
	/*if (vertexUsage & (1UL << DVertexUsage_TEXCOORD0))
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
	}*/
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

void DMeshRes10::OnRefresh(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
}

bool DMeshRes10::OnInit(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	D3D10_BUFFER_DESC  vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = m_isDynamic ? D3D10_USAGE_DYNAMIC : D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_dataSize * vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = m_isDynamic ? D3D10_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;

	vertexData.pSysMem = vertexbuffer;

	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = m_isDynamic ? D3D10_USAGE_DYNAMIC : D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = m_isDynamic ? D3D10_CPU_ACCESS_WRITE : 0;
	indexBufferDesc.MiscFlags = 0;

	indexData.pSysMem = indexbuffer;

	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
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
