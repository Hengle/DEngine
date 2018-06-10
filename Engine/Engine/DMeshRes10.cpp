#include "DMeshRes10.h"

DMeshRes10::DMeshRes10(ID3D10Device * device) : DMeshRes()
{
	m_device = device;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_dataSize = 0;
	m_indexCount = 0;
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
	D3D10_BUFFER_DESC  vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_dataSize = desc->dataSize;
	m_indexCount = desc->indexCount;

	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_dataSize * desc->vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	vertexData.pSysMem = desc->vertices;

	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
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
		return false;
	}
	return true;
}

void DMeshRes10::OnDraw()
{
	unsigned int stride;
	unsigned int offset;


	stride = m_dataSize;
	offset = 0;

	ID3D10Buffer* vbuffer = m_vertexBuffer;
	ID3D10Buffer* ibuffer = m_indexBuffer;

	m_device->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_device->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_device->DrawIndexed(m_indexCount, 0, 0);
}
