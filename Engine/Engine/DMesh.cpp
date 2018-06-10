#include "DMesh.h"
#include "DModelLoader.h"
#include "DSystem.h"

DMesh::DMesh()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_dataSize = 0;
	//m_meshBuffer = 0;
	m_meshRes = 0;
	m_vertexOffset = 0;
	m_normalOffset = 0;
	m_colorOffset = 0;
	m_uvOffset = 0;

	m_vertexChanged = false;
}


DMesh::~DMesh()
{
}

void DMesh::Destroy()
{
	if (m_meshRes != NULL)
	{
		m_meshRes->Release();
		delete m_meshRes;
		m_meshRes = NULL;
	}
	/*if (m_meshBuffer != NULL)
	{
		m_meshBuffer->Release();
		delete m_meshBuffer;
		m_meshBuffer = NULL;
	}*/
	if (m_vertexBuffer != NULL)
	{
		delete[] m_vertexBuffer;
		m_vertexBuffer = 0;
	}
	if (m_indexBuffer != NULL)
	{
		delete[] m_indexBuffer;
		m_indexBuffer = 0;
	}
}

int DMesh::GetIndexCount() const
{
	return m_indexCount;
}

int DMesh::GetVertexCount() const
{
	return m_vertexCount;
}

void DMesh::GetVertex(int index, DVector3 * vertex) const
{
	float x = m_vertexBuffer[index*m_vertexOffset];
	float y = m_vertexBuffer[index*m_vertexOffset + 1];
	float z = m_vertexBuffer[index*m_vertexOffset + 2];
	vertex->x = x;
	vertex->y = y;
	vertex->z = z;
}

void DMesh::GetNormal(int index, DVector3 * normal) const
{
	if (m_normalOffset <= 0)
		return;
	float x = m_vertexBuffer[index*m_vertexOffset+m_normalOffset];
	float y = m_vertexBuffer[index*m_vertexOffset+m_normalOffset + 1];
	float z = m_vertexBuffer[index*m_vertexOffset+m_normalOffset + 2];
	normal->x = x;
	normal->y = y;
	normal->z = z;
}

void DMesh::GetColor(int index, DColor * color) const
{
	if (m_colorOffset <= 0)
		return;
	float r = m_vertexBuffer[index*m_vertexOffset + m_colorOffset];
	float g = m_vertexBuffer[index*m_vertexOffset + m_colorOffset + 1];
	float b = m_vertexBuffer[index*m_vertexOffset + m_colorOffset + 2];
	float a = m_vertexBuffer[index*m_vertexOffset + m_colorOffset + 3];
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;
}

void DMesh::GetUV(int index, int channel, DVector2 * uv) const
{
	if (m_uvOffset <= 0)
		return;
	float x = m_vertexBuffer[index*m_vertexOffset + m_uvOffset];
	float y = m_vertexBuffer[index*m_vertexOffset + m_uvOffset + 1];
	uv->x = x;
	uv->y = y;
}

void DMesh::SetVertex(int index, const DVector3 & vertex)
{
	m_vertexBuffer[index*m_vertexOffset] = vertex.x;
	m_vertexBuffer[index*m_vertexOffset + 1] = vertex.y;
	m_vertexBuffer[index*m_vertexOffset + 2] = vertex.z;
	m_vertexChanged = true;
}

void DMesh::SetNormal(int index, const DVector3 & normal)
{
	if (m_normalOffset <= 0)
		return;
	m_vertexBuffer[index*m_vertexOffset + m_normalOffset] = normal.x;
	m_vertexBuffer[index*m_vertexOffset + m_normalOffset + 1] = normal.y;
	m_vertexBuffer[index*m_vertexOffset + m_normalOffset + 2] = normal.z;
	m_vertexChanged = true;
}

void DMesh::SetColor(int index, const DColor & color)
{
	if (m_colorOffset <= 0)
		return;
	m_vertexBuffer[index*m_vertexOffset + m_colorOffset] = color.r;
	m_vertexBuffer[index*m_vertexOffset + m_colorOffset + 1] = color.g;
	m_vertexBuffer[index*m_vertexOffset + m_colorOffset + 2] = color.b;
	m_vertexBuffer[index*m_vertexOffset + m_colorOffset + 3] = color.a;
	m_vertexChanged = true;
}

void DMesh::SetUV(int index, int channel, const DVector3 & uv)
{
	if (m_uvOffset <= 0)
		return;
	m_vertexBuffer[index*m_vertexOffset + m_uvOffset] = uv.x;
	m_vertexBuffer[index*m_vertexOffset + m_uvOffset + 1] = uv.y;
	m_vertexChanged = true;
}

void DMesh::GetIndex(int index, unsigned long & outIndex) const
{
	outIndex = m_indexBuffer[index];
}

bool DMesh::HasNormal() const
{
	return m_normalOffset > 0;
}

bool DMesh::HasColor() const
{
	return m_colorOffset > 0;
}

bool DMesh::HasUV(int channel) const
{
	return m_uvOffset > 0;
}

void DMesh::Draw() const
{
	if (m_meshRes != NULL)
	{
		m_meshRes->Draw();
	}
}

//int DMesh::GetDataSize() const
//{
//	return m_dataSize;
//}

//DMeshBuffer * DMesh::GetBuffer() const
//{
//	return m_meshBuffer;
//}

DMesh * DMesh::Create(DMeshDefine meshDefine)
{
	float* vertices;
	unsigned long* indices;
	int vcount, icount, dsize, blen;
	if (meshDefine == DMESH_Plane)
	{
		DModelLoader::CreatePlane(&vertices, &indices, vcount, icount, blen, dsize);
	}
	else
		return NULL;

	DMeshBufferDesc desc;
	desc.dataCount = blen;
	desc.dataSize = dsize;
	desc.indexCount = icount;
	desc.vertexCount = vcount;
	desc.indices = indices;
	desc.vertices = vertices;

	return Create(&desc);
}

DMesh * DMesh::Create(char* fileName)
{
	float* vertices;
	unsigned long* indices;
	int vcount, icount, dsize, blen;
	DModelLoader::LoadObj(fileName, &vertices, &indices, vcount, icount, blen, dsize);

	DMeshBufferDesc desc;
	desc.dataCount = blen;
	desc.dataSize = dsize;
	desc.indexCount = icount;
	desc.vertexCount = vcount;
	desc.indices = indices;
	desc.vertices = vertices;

	return Create(&desc);
}

DMesh * DMesh::Create(DMeshBufferDesc * desc)
{
	if (desc == NULL)
		return nullptr;
	if (desc->dataCount == 0 || desc->dataSize == 0 || desc->indexCount <= 0 || desc->vertexCount <= 0)
		return nullptr;
	if (desc->vertices == 0 || desc->indices == 0)
		return nullptr;
	DMesh* mesh = new DMesh();
	mesh->Init(desc);
	
	return nullptr;
}

void DMesh::Init(DMeshBufferDesc* desc)
{
	m_vertexBuffer = desc->vertices;
	m_indexBuffer = desc->indices;
	m_vertexCount = desc->vertexCount;
	m_indexCount = desc->indexCount;
	m_dataSize = desc->dataSize;
	m_bufferLength = desc->dataCount;
	m_vertexOffset = 8;
	m_uvOffset = 3;
	m_normalOffset = 5;

	m_meshRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateMeshRes();
	m_meshRes->Init(desc);
}

