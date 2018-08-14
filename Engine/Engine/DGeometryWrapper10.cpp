#ifdef _DGAPI_D3D10
#include "DGeometryWrapper10.h"

DGeometryWrapper10::DGeometryWrapper10(ID3D10Device * device, int vertexUsage, bool dynamic) : DGeometryWrapper(vertexUsage, dynamic)
{
	m_device = device;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

}

DGeometryWrapper10::~DGeometryWrapper10()
{
}

void DGeometryWrapper10::Release()
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

void DGeometryWrapper10::OnRefresh(float * vertexbuffer, unsigned int * indexbuffer, int vertexCount, int indexCount)
{

	void* pdata;
	m_vertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &pdata);
	float* dataPtr = (float*)pdata;
	memcpy(dataPtr, vertexbuffer, m_dataSize* vertexCount);
	m_vertexBuffer->Unmap();

	void* idata;
	m_indexBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &idata);
	unsigned int* idataPtr = (unsigned int*)idata;
	memcpy(idataPtr, indexbuffer, sizeof(unsigned int)*indexCount);
	m_indexBuffer->Unmap();
}

bool DGeometryWrapper10::OnInit(float * vertexbuffer, unsigned int * indexbuffer, int vertexCount, int indexCount)
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
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
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

void DGeometryWrapper10::OnDraw(DGeometryTopology topology)
{
	unsigned int stride;
	unsigned int offset;


	stride = m_dataSize;
	offset = 0;

	ID3D10Buffer* vbuffer = m_vertexBuffer;
	ID3D10Buffer* ibuffer = m_indexBuffer;

	m_device->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_device->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	if (topology == DGeometryTopology_LineList)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	else if (topology == DGeometryTopology_LineStrip)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	else if (topology == DGeometryTopology_PointList)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	else if (topology == DGeometryTopology_TriangleList)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (topology == DGeometryTopology_TriangleStrip)
		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_device->DrawIndexed(m_indexCount, 0, 0);
}
#endif