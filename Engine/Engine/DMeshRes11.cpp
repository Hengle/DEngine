#include "DMeshRes11.h"

DMeshRes11::DMeshRes11(ID3D11Device* device,ID3D11DeviceContext * deviceContext, int vertexUsage, bool dynamic) : DMeshRes(vertexUsage, dynamic)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_device = device;
	m_deviceContext = deviceContext;
	//m_dataSize = 0;
	//m_dataCount = 0;
	//m_indexCount = 0;

	m_dataSize = sizeof(float) * m_dataCount;
	//m_dataCount = 3;
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

DMeshRes11::~DMeshRes11()
{
}

void DMeshRes11::Release()
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
	m_deviceContext = NULL;
}

void DMeshRes11::OnRefresh(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	float* dataPtr;
	unsigned i;
	m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (float*)mappedResource.pData;
	memcpy(dataPtr, vertexbuffer, m_dataSize* vertexCount);
	//for (i = 0; i < m_dataCount*vertexCount; i++)
	//{
	//	dataPtr[i] = vertexbuffer[i];
	//}
	m_deviceContext->Unmap(m_vertexBuffer, 0);

	D3D11_MAPPED_SUBRESOURCE imappedResource;
	unsigned long* idataPtr;
	m_deviceContext->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &imappedResource);
	idataPtr = (unsigned long*)imappedResource.pData;
	memcpy(idataPtr, indexbuffer, sizeof(unsigned long)*indexCount);
	//for (i = 0; i < indexCount; i++)
	//{
	//	idataPtr[i] = indexbuffer[i];
	//}
	m_deviceContext->Unmap(m_indexBuffer, 0);
}

bool DMeshRes11::OnInit(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = m_isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_dataSize * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = m_isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertexbuffer;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = m_isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = m_isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indexbuffer;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void DMeshRes11::OnDraw(DMeshTopology topology)
{
	unsigned int stride;
	unsigned int offset;


	stride = m_dataSize;
	offset = 0;

	ID3D11Buffer* vbuffer = m_vertexBuffer;
	ID3D11Buffer* ibuffer = m_indexBuffer;

	m_deviceContext->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_deviceContext->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	if(topology == DMeshTopology_LineList)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	else if(topology == DMeshTopology_LineStrip)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	else if (topology == DMeshTopology_PointList)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	else if (topology == DMeshTopology_TriangleList)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (topology == DMeshTopology_TriangleStrip)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	

	m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
}
