#include "DMeshRes9.h"

DMeshRes9::DMeshRes9(LPDIRECT3DDEVICE9 device) : DMeshRes()
{
	m_device = device;
}

DMeshRes9::~DMeshRes9()
{
}

bool DMeshRes9::OnInit(DMeshBufferDesc * desc)
{
	if (desc == NULL)
		return false;
	if (desc->indexCount <= 0 || desc->vertexCount <= 0 || desc->dataSize <= 0 || desc->dataCount <= 0)
		return false;
	if (desc->vertices == nullptr)
		return false;
	if (desc->indices == nullptr)
		return false;
	D3DVERTEXELEMENT9 elements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	m_dataSize = desc->dataSize;
	m_vertexCount = desc->vertexCount;
	m_indexCount = desc->indexCount;
	m_pimCount = desc->indexCount / 3;
	int vlength = desc->vertexCount*desc->dataCount;

	D3DXCreateMesh(m_pimCount, m_vertexCount, D3DXMESH_MANAGED, elements, m_device, &m_mesh);
	//m_vertexBuffer->Lock()
	float* ves;
	int i;
	m_mesh->LockVertexBuffer(0, (void**)&ves);

	for (i = 0; i < vlength; i++)
	{
		ves[i] = desc->vertices[i];
	}

	m_mesh->UnlockVertexBuffer();

	WORD* ids = 0;
	m_mesh->LockIndexBuffer(0, (void**)&ids);

	for (i = 0; i < m_indexCount; i++)
	{
		ids[i] = desc->indices[i];
	}

	m_mesh->UnlockIndexBuffer();

	return true;
}

void DMeshRes9::OnDraw()
{
	m_mesh->DrawSubset(0);
}

void DMeshRes9::Release()
{
	m_device = NULL;
	if (m_mesh != NULL)
	{
		m_mesh->Release();
	}
	m_mesh = NULL;
}
