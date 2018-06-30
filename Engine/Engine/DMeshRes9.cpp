#include "DMeshRes9.h"
#include <vector>

DMeshRes9::DMeshRes9(LPDIRECT3DDEVICE9 device, int vertexUsage, bool dynamic) : DMeshRes(vertexUsage, dynamic)
{
	m_device = device;
	//m_mesh = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_vertexDeclaration = 0;

	//m_dataSize = 0;
	//m_dataCount = 0;
	//m_triCount = 0;
	//m_indexCount = 0;
	//m_vertexCount = 0;

	//float fsize = sizeof(float);
	//m_dataSize = fsize * 3;
	//m_dataCount = 3;
	//if (vertexUsage & (1UL << DVertexUsage_TEXCOORD0))
	//{
	//	//m_dataSize += fsize * 2;
	//	m_dataCount += 2;
	//	m_hasUV = true;
	//}
	//if (vertexUsage & (1UL << DVertexUsage_TEXCOORD1))
	//{
	//	//m_dataSize += fsize * 2;
	//	m_dataCount += 2;
	//	m_hasUV1 = true;
	//}
	//if (vertexUsage & (1UL << DVertexUsage_TEXCOORD2))
	//{
	//	//m_dataSize += fsize * 2;
	//	m_dataCount += 2;
	//	m_hasUV2 = true;
	//}
	//if (vertexUsage & (1UL << DVertexUsage_TEXCOORD3))
	//{
	//	//m_dataSize += fsize * 2;
	//	m_dataCount += 2;
	//	m_hasUV3 = true;
	//}
	//if (vertexUsage & (1UL << DVertexUsage_NORMAL))
	//{
	//	//m_dataSize += fsize * 3;
	//	m_dataCount += 3;
	//	m_hasNormal = true;
	//}
	//if (vertexUsage & (1UL << DVertexUsage_COLOR))
	//{
	//	//m_dataSize += fsize * 4;
	//	m_dataCount += 4;
	//	m_hasColor = true;
	//}
	//if (vertexUsage & (1UL << DVertexUsage_TANGENT))
	//{
	//	//m_dataSize += fsize * 4;
	//	m_dataCount += 4;
	//	m_hasTangent = true;
	//}
	//if (vertexUsage & (1UL << DVertexUsage_BINORMAL))
	//{
	//	//m_dataSize += fsize * 3;
	//	m_dataCount += 3;
	//	m_hasBinormal = true;
	//}
}

DMeshRes9::~DMeshRes9()
{
}

void DMeshRes9::OnRefresh(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	return;
	float* vertices;
	int i;
	int vlen = m_dataCount*vertexCount;

	/*m_vertexBuffer->Lock(0, 0, (void**)vertices, 0);
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
	m_indexBuffer->Unlock();*/

	/*m_mesh->LockVertexBuffer(0, (void**)&vertices);

	for (i = 0; i < vlen; i++)
	{
		vertices[i] = vertexbuffer[i];
	}

	m_mesh->UnlockVertexBuffer();

	WORD* ids = 0;
	m_mesh->LockIndexBuffer(0, (void**)&ids);

	for (i = 0; i < m_indexCount; i++)
	{
		ids[i] = indexbuffer[i];
	}

	m_mesh->UnlockIndexBuffer();*/
}

bool DMeshRes9::OnInit(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
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

	//m_dataSize = desc->dataSize;
	

	m_device->CreateVertexDeclaration(&elements[0], &m_vertexDeclaration);
	UINT len = m_dataSize*vertexCount;
	m_device->CreateVertexBuffer(len, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_vertexBuffer, 0);
	m_device->CreateIndexBuffer(sizeof(WORD)*indexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuffer, 0);

	//D3DXCreateMesh(tcount, vertexCount, D3DXMESH_MANAGED, &elements[0], m_device, &m_mesh);

	//m_vertexBuffer->Lock()
	float* vertices;
	int i;
	int vlen = m_dataCount*vertexCount;
	//m_mesh->LockVertexBuffer(0, (void**)&vertices);
	m_vertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	for (i = 0; i < vlen; i++)
	{
		vertices[i] = vertexbuffer[i];
	}

	//m_mesh->UnlockVertexBuffer();
	m_vertexBuffer->Unlock();

	WORD* ids = 0;
	//m_mesh->LockIndexBuffer(0, (void**)&ids);
	m_indexBuffer->Lock(0, 0, (void**)&ids, 0);

	for (i = 0; i < m_indexCount; i++)
	{
		ids[i] = indexbuffer[i];
	}

	//m_mesh->UnlockIndexBuffer();
	m_indexBuffer->Unlock();

	elements.clear();

	return true;
}

void DMeshRes9::OnDraw(DMeshTopology topology)
{
	//if(m_mesh != NULL)
	//	m_mesh->DrawSubset(0);
	m_device->SetStreamSource(0, m_vertexBuffer, 0, m_dataSize);
	m_device->SetIndices(m_indexBuffer);
	m_device->SetVertexDeclaration(m_vertexDeclaration);

	if (topology == DMeshTopology_LineList)
		m_device->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_vertexCount, 0, m_indexCount / 2);
	else if (topology == DMeshTopology_LineStrip)
		m_device->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, m_vertexCount, 0, m_indexCount / 2);
	else if (topology == DMeshTopology_PointList)
		m_device->DrawIndexedPrimitive(D3DPT_POINTLIST, 0, 0, m_vertexCount, 0, m_indexCount);
	else if (topology == DMeshTopology_TriangleList) {
		m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_indexCount / 3);
	}
	else if (topology == DMeshTopology_TriangleStrip)
		m_device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_vertexCount, 0, m_indexCount / 3);

	//m_device->SetStreamSource()
	//Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	//Device->SetIndices(IB);
	//Device->SetFVF(Vertex::FVF);

	//// Draw cube.
	//Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void DMeshRes9::Release()
{
	m_device = NULL;
	/*if (m_mesh != NULL)
	{
		m_mesh->Release();
	}
	m_mesh = NULL;*/
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
