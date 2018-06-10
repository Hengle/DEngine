#include "DMeshRes11.h"

DMeshRes11::DMeshRes11(ID3D11Device* device,ID3D11DeviceContext * deviceContext) : DMeshRes()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_device = device;
	m_deviceContext = deviceContext;
	m_dataSize = 0;
	m_indexCount = 0;
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

bool DMeshRes11::OnInit(DMeshBufferDesc * desc)
{
	if (desc == NULL)
		return false;
	if (desc->indexCount <= 0 || desc->vertexCount <= 0 || desc->dataSize <= 0 || desc->dataCount <= 0)
		return false;
	if (desc->vertices == nullptr)
		return false;
	if (desc->indices == nullptr)
		return false;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = desc->dataSize * desc->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = desc->vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	m_dataSize = desc->dataSize;
	m_indexCount = desc->indexCount;

	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * desc->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = desc->indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void DMeshRes11::OnDraw()
{
	unsigned int stride;
	unsigned int offset;


	stride = m_dataSize;
	offset = 0;

	ID3D11Buffer* vbuffer = m_vertexBuffer;
	ID3D11Buffer* ibuffer = m_indexBuffer;

	m_deviceContext->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_deviceContext->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
}
