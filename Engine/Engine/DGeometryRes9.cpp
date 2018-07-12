#include "DGeometryRes9.h"
#include <vector>

DGeometryRes9::DGeometryRes9(LPDIRECT3DDEVICE9 device, int vertexUsage, bool dynamic) : DGeometryRes(vertexUsage, dynamic)
{
	m_device = device;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_vertexDeclaration = 0;
}

DGeometryRes9::~DGeometryRes9()
{
}

void DGeometryRes9::OnRefresh(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	return;
	float* vertices;
	int i;
	int vlen = m_dataCount*vertexCount;

	m_vertexBuffer->Lock(0, 0, (void**)vertices, 0);
	for (i = 0; i < vlen; i++)
	{
		vertices[i] = vertexbuffer[i];
	}
	m_vertexBuffer->Unlock();

	WORD* ids = 0;
	m_indexBuffer->Lock(0, 0, (void**)&ids, 0);
	for (i = 0; i < m_indexCount; i++)
	{
		ids[i] = indexbuffer[i];
	}
	m_indexBuffer->Unlock();
}

bool DGeometryRes9::OnInit(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	std::vector<D3DVERTEXELEMENT9> elements;
	WORD offset = 0;
	elements.push_back({ 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 });
	offset += 12;
	if (m_hasNormal)
	{
		elements.push_back({ 0, offset, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 });
		offset += 12;
	}
	if (m_hasColor)
	{
		elements.push_back({ 0, offset, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 });
		offset += 16;
	}
	if (m_hasUV)
	{
		elements.push_back({ 0, offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 });
		offset += 8;
	}

	elements.push_back(D3DDECL_END());
	

	m_device->CreateVertexDeclaration(&elements[0], &m_vertexDeclaration);
	UINT len = m_dataSize*vertexCount;
	m_device->CreateVertexBuffer(len, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_vertexBuffer, 0);
	m_device->CreateIndexBuffer(sizeof(WORD)*indexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuffer, 0);

	float* vertices;
	int i;
	int vlen = m_dataCount*vertexCount;
	m_vertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	for (i = 0; i < vlen; i++)
	{
		vertices[i] = vertexbuffer[i];
	}

	m_vertexBuffer->Unlock();

	WORD* ids = 0;
	m_indexBuffer->Lock(0, 0, (void**)&ids, 0);

	for (i = 0; i < m_indexCount; i++)
	{
		ids[i] = indexbuffer[i];
	}

	m_indexBuffer->Unlock();

	elements.clear();

	return true;
}

void DGeometryRes9::OnDraw(DGeometryTopology topology)
{
	m_device->SetStreamSource(0, m_vertexBuffer, 0, m_dataSize);
	m_device->SetIndices(m_indexBuffer);
	m_device->SetVertexDeclaration(m_vertexDeclaration);

	if (topology == DGeometryTopology_LineList)
		m_device->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_vertexCount, 0, m_indexCount / 2);
	else if (topology == DGeometryTopology_LineStrip)
		m_device->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, m_vertexCount, 0, m_indexCount / 2);
	else if (topology == DGeometryTopology_PointList)
		m_device->DrawIndexedPrimitive(D3DPT_POINTLIST, 0, 0, m_vertexCount, 0, m_indexCount);
	else if (topology == DGeometryTopology_TriangleList) {
		m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_indexCount / 3);
	}
	else if (topology == DGeometryTopology_TriangleStrip)
		m_device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_vertexCount, 0, m_indexCount / 3);

}

void DGeometryRes9::Release()
{
	m_device = NULL;

	if (m_vertexBuffer != NULL)
	{
		m_vertexBuffer->Release();
	}
	if (m_indexBuffer != NULL)
	{
		m_indexBuffer->Release();
	}
	if (m_vertexDeclaration != NULL)
	{
		m_vertexDeclaration->Release();
	}
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
	m_vertexDeclaration = NULL;
}
