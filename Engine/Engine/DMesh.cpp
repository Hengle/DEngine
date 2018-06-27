#include "DMesh.h"
#include "DModelLoader.h"
#include "DSystem.h"

DMesh::DMesh(bool dynamic)
{
	//m_vertexBuffer = 0;
	//m_indexBuffer = 0;
	//m_dataSize = 0;
	//m_meshBuffer = 0;
	//m_meshRes = 0;
	//m_vertexOffset = 0;
	//m_normalOffset = 0;
	//m_colorOffset = 0;
	//m_uvOffset = 0;

	m_vertexChanged = false;
	m_meshDesc.vertices = 0;
	m_meshDesc.uvs = 0;
	m_meshDesc.uv2s = 0;
	m_meshDesc.uv3s = 0;
	m_meshDesc.normals = 0;
	m_meshDesc.colors = 0;
	m_meshDesc.indices = 0;
	/*m_vertices = 0;
	m_uvs = 0;
	m_uv2s = 0;
	m_uv3s = 0;
	m_normals = 0;
	m_colors = 0;
	m_indices = 0;*/

	m_topology = DMeshTopology_TriangleList;
	m_dynamic = dynamic;
}


DMesh::~DMesh()
{
}

void DMesh::Destroy()
{
	std::map<int, DMeshRes*>::iterator  iter;
	for (iter = m_meshReses.begin(); iter != m_meshReses.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
			delete iter->second;
		}
	}
	m_meshReses.clear();
	/*if (m_meshRes != NULL)
	{
		m_meshRes->Release();
		delete m_meshRes;
		m_meshRes = NULL;
	}*/
	/*if (m_meshBuffer != NULL)
	{
		m_meshBuffer->Release();
		delete m_meshBuffer;
		m_meshBuffer = NULL;
	}*/
	/*if (m_vertexBuffer != NULL)
	{
		delete[] m_vertexBuffer;
		m_vertexBuffer = 0;
	}
	if (m_indexBuffer != NULL)
	{
		delete[] m_indexBuffer;
		m_indexBuffer = 0;
	}*/
	if (m_meshDesc.vertices != NULL)
	{
		delete[] m_meshDesc.vertices;
		m_meshDesc.vertices = 0;
	}
	if (m_meshDesc.uvs != NULL)
	{
		delete[] m_meshDesc.uvs;
		m_meshDesc.uvs = 0;
	}
	if (m_meshDesc.uv2s != NULL)
	{
		delete[] m_meshDesc.uv2s;
		m_meshDesc.uv2s = 0;
	}
	if (m_meshDesc.uv3s != NULL)
	{
		delete[] m_meshDesc.uv3s;
		m_meshDesc.uv3s = 0;
	}
	if (m_meshDesc.normals != NULL)
	{
		delete[] m_meshDesc.normals;
		m_meshDesc.normals = 0;
	}
	if (m_meshDesc.colors != NULL)
	{
		delete[] m_meshDesc.colors;
		m_meshDesc.colors = 0;
	}
	if (m_meshDesc.indices != NULL)
	{
		delete[] m_meshDesc.indices;
		m_meshDesc.indices = 0;
	}
}

int DMesh::GetIndexCount() const
{
	return m_meshDesc.indexCount;
}

int DMesh::GetVertexCount() const
{
	return m_meshDesc.vertexCount;
}

void DMesh::GetVertex(int index, DVector3 * vertex) const
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (m_meshDesc.vertices == 0)
		return;
	float x = m_meshDesc.vertices[index*3];
	float y = m_meshDesc.vertices[index*3 + 1];
	float z = m_meshDesc.vertices[index*3 + 2];
	vertex->x = x;
	vertex->y = y;
	vertex->z = z;
}

void DMesh::GetNormal(int index, DVector3 * normal) const
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (m_meshDesc.normals == 0)
		return;
	float x = m_meshDesc.normals[index*3];
	float y = m_meshDesc.normals[index*3 + 1];
	float z = m_meshDesc.normals[index*3 + 2];
	normal->x = x;
	normal->y = y;
	normal->z = z;
}

void DMesh::GetColor(int index, DColor * color) const
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (m_meshDesc.colors == 0)
		return;
	float r = m_meshDesc.colors[index*4];
	float g = m_meshDesc.colors[index*4 + 1];
	float b = m_meshDesc.colors[index*4 + 2];
	float a = m_meshDesc.colors[index*4 + 3];
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;
}

void DMesh::GetUV(int index, int channel, DVector2 * uv) const
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (channel == 0)
	{
		if (m_meshDesc.uvs == 0)
			return;
		uv->x = m_meshDesc.uvs[index*2];
		uv->y = m_meshDesc.uvs[index*2+1];
	}
	else if (channel == 1)
	{
		if (m_meshDesc.uv2s == 0)
			return;
		uv->x = m_meshDesc.uv2s[index * 2];
		uv->y = m_meshDesc.uv2s[index * 2 + 1];
	}
	else if (channel == 1)
	{
		if (m_meshDesc.uv3s == 0)
			return;
		uv->x = m_meshDesc.uv3s[index * 2];
		uv->y = m_meshDesc.uv3s[index * 2 + 1];
	}
}

DMeshTopology DMesh::GetTopology()
{
	return m_topology;
}

void DMesh::SetVertex(int index, const DVector3 & vertex)
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (m_meshDesc.vertices == 0)
		return;
	m_meshDesc.vertices[index*3] = vertex.x;
	m_meshDesc.vertices[index*3 + 1] = vertex.y;
	m_meshDesc.vertices[index*3 + 2] = vertex.z;
	m_vertexChanged = true;
}

void DMesh::SetNormal(int index, const DVector3 & normal)
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (m_meshDesc.normals == 0)
		return;
	m_meshDesc.normals[index*3] = normal.x;
	m_meshDesc.normals[index*3 + 1] = normal.y;
	m_meshDesc.normals[index*3 + 2] = normal.z;
	m_vertexChanged = true;
}

void DMesh::SetColor(int index, const DColor & color)
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (m_meshDesc.colors == 0)
		return;
	m_meshDesc.colors[index*4] = color.r;
	m_meshDesc.colors[index*4 + 1] = color.g;
	m_meshDesc.colors[index*4 + 2] = color.b;
	m_meshDesc.colors[index*4 + 3] = color.a;
	m_vertexChanged = true;
}

void DMesh::SetVertices(DVector3 * vertices, int length)
{
	unsigned int i;
	if (m_meshDesc.vertices != 0)
	{
		delete[] m_meshDesc.vertices;
		m_meshDesc.vertices = 0;
	}
	m_meshDesc.vertices = new float[length * 3];
	for (i = 0; i < length; i++)
	{
		m_meshDesc.vertices[i * 3] = vertices[i].x;
		m_meshDesc.vertices[i * 3 + 1] = vertices[i].y;
		m_meshDesc.vertices[i * 3 + 2] = vertices[i].z;
	}
	m_meshDesc.vertexCount = length;
	m_vertexChanged = true;
}

void DMesh::SetVertices(float * vertices, int vertexCount)
{
	if (m_meshDesc.vertices != 0)
	{
		delete[] m_meshDesc.vertices;
		m_meshDesc.vertices = 0;
	}
	m_meshDesc.vertices = vertices;
	m_meshDesc.vertexCount = vertexCount;
	m_vertexChanged = true;
}

void DMesh::SetUVs(int channel, DVector2 * uvs, int length)
{
	if (m_meshDesc.vertexCount != length)
		return;
	if (channel == 0)
	{
		unsigned int i;
		if (m_meshDesc.uvs != 0)
		{
			delete[] m_meshDesc.uvs;
			m_meshDesc.uvs = 0;
		}
		m_meshDesc.uvs = new float[length * 2];
		for (i = 0; i < length; i++)
		{
			m_meshDesc.uvs[i * 3] = uvs[i].x;
			m_meshDesc.uvs[i * 3 + 1] = uvs[i].y;
		}
		m_vertexChanged = true;
	}
	else if (channel == 1)
	{
		unsigned int i;
		if (m_meshDesc.uv2s != 0)
		{
			delete[] m_meshDesc.uv2s;
			m_meshDesc.uv2s = 0;
		}
		m_meshDesc.uv2s = new float[length * 2];
		for (i = 0; i < length; i++)
		{
			m_meshDesc.uv2s[i * 3] = uvs[i].x;
			m_meshDesc.uv2s[i * 3 + 1] = uvs[i].y;
		}
		m_vertexChanged = true;
	}
}

void DMesh::SetUVs(int channel, float * uvs, int length)
{
	if (m_meshDesc.vertexCount != length)
		return;
	if (channel == 0)
	{
		unsigned int i;
		if (m_meshDesc.uvs != 0)
		{
			delete[] m_meshDesc.uvs;
			m_meshDesc.uvs = 0;
		}
		m_meshDesc.uvs = uvs;
		m_vertexChanged = true;
	}
}

void DMesh::SetNormals(DVector3 *normals, int length)
{
	if (m_meshDesc.vertexCount != length)
		return;
	unsigned int i;
	if (m_meshDesc.normals != 0)
	{
		delete[] m_meshDesc.normals;
		m_meshDesc.normals = 0;
	}
	m_meshDesc.normals = new float[length * 3];
	for (i = 0; i < length; i++)
	{
		m_meshDesc.normals[i * 3] = normals[i].x;
		m_meshDesc.normals[i * 3 + 1] = normals[i].y;
		m_meshDesc.normals[i * 3 + 2] = normals[i].z;
	}
	m_vertexChanged = true;
}

void DMesh::SetColors(DColor * colors, int length)
{
	if (m_meshDesc.vertexCount != length)
		return;
	unsigned int i;
	if (m_meshDesc.colors != 0)
	{
		delete[] m_meshDesc.colors;
		m_meshDesc.colors = 0;
	}
	m_meshDesc.colors = new float[length * 4];
	for (i = 0; i < length; i++)
	{
		m_meshDesc.colors[i * 4] = colors[i].r;
		m_meshDesc.colors[i * 4 + 1] = colors[i].g;
		m_meshDesc.colors[i * 4 + 2] = colors[i].b;
		m_meshDesc.colors[i * 4 + 3] = colors[i].a;
	}
	m_vertexChanged = true;
}

void DMesh::SetIndices(unsigned long * indices, int length)
{
	if (m_meshDesc.indices != 0)
	{
		delete[] m_meshDesc.indices;
		m_meshDesc.indices = 0;
	}
	m_meshDesc.indices = indices;
	m_meshDesc.indexCount = length;
	m_vertexChanged = true;
}

void DMesh::SetUV(int index, int channel, const DVector2 & uv)
{
	if (index < 0 || index >= m_meshDesc.vertexCount)
		return;
	if (channel == 0)
	{
		if (m_meshDesc.uvs == 0)
			return;
		m_meshDesc.uvs[index*2] = uv.x;
		m_meshDesc.uvs[index*2 + 1] = uv.y;
		m_vertexChanged = true;
	}
	else if (channel == 1)
	{
		if (m_meshDesc.uv2s == 0)
			return;
		m_meshDesc.uv2s[index * 2] = uv.x;
		m_meshDesc.uv2s[index * 2 + 1] = uv.y;
		m_vertexChanged = true;
	}
	else if (channel == 2)
	{
		if (m_meshDesc.uv3s == 0)
			return;
		m_meshDesc.uv3s[index * 2] = uv.x;
		m_meshDesc.uv3s[index * 2 + 1] = uv.y;
		m_vertexChanged = true;
	}
}

void DMesh::SetTopology(DMeshTopology topology)
{
	m_topology = topology;
}

void DMesh::GetIndex(int index, unsigned long & outIndex) const
{
	if (index < 0 || index >= m_meshDesc.indexCount)
		return;
	outIndex = m_meshDesc.indices[index];
}

bool DMesh::HasNormal() const
{
	return m_meshDesc.normals != 0;
}

bool DMesh::HasColor() const
{
	return m_meshDesc.colors != 0;
}

bool DMesh::HasUV(int channel) const
{
	if (channel == 0)
		return m_meshDesc.uvs != 0;
	if (channel == 1)
		return m_meshDesc.uv2s != 0;
	if (channel == 2)
		return m_meshDesc.uv3s != 0;
	return false;
}

void DMesh::Draw(int vertexUsage)
{
	if (vertexUsage <= 0)
		return;
	if (m_meshReses.find(vertexUsage) == m_meshReses.end())
	{
		DMeshRes* res = DSystem::GetGraphicsMgr()->GetGLCore()->CreateMeshRes(vertexUsage, m_dynamic);
		//res->Init(&m_meshDesc, vertexUsage);
		m_meshReses.insert(std::pair<int, DMeshRes*>(vertexUsage, res));
	}
	if (m_vertexChanged)
	{
		UpdateMeshReses();
		m_vertexChanged = false;
	}
	/*if (m_meshRes != NULL)
	{
		m_meshRes->Draw(m_topology);
	}*/
	if (m_meshReses.find(vertexUsage) != m_meshReses.end())
	{
		DMeshRes* res = m_meshReses.at(vertexUsage);
		if (!res->IsInitialized())
		{
			res->Refresh(&m_meshDesc);
		}
		res->Draw(m_topology);
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

DMesh * DMesh::Create(DMeshDefine meshDefine, bool dynamic)
{
	/*float* vertices;
	unsigned long* indices;
	int vcount, icount, dsize, blen;*/
	DMeshBufferDesc desc;
	if (meshDefine == DMESH_Plane)
	{
		DModelLoader::CreatePlane(&desc);
	}
	else if (meshDefine == DMESH_Cube)
	{
		DModelLoader::CreateCube(&desc);
	}
	else
		return NULL;

	
	/*desc.dataCount = blen;
	desc.dataSize = dsize;
	desc.indexCount = icount;
	desc.vertexCount = vcount;
	desc.indices = indices;
	desc.vertices = vertices;*/
	DMesh* mesh = new DMesh(dynamic);
	mesh->m_meshDesc = desc;

	return mesh;
}

DMesh * DMesh::Create(char* fileName, bool dynamic)
{
	/*float* vertices;
	unsigned long* indices;
	int vcount, icount, dsize, blen;*/
	DMeshBufferDesc desc;
	DModelLoader::LoadObj(fileName, &desc);

	DMesh* mesh = new DMesh(dynamic);
	mesh->m_meshDesc = desc;

	return mesh;
	/*desc.dataCount = blen;
	desc.dataSize = dsize;
	desc.indexCount = icount;
	desc.vertexCount = vcount;
	desc.indices = indices;
	desc.vertices = vertices;*/

	//return Create(&desc);
}

//DMesh * DMesh::Create(DMeshBufferDesc * desc)
//{
//	if (desc == NULL)
//		return nullptr;
//	if (desc->dataCount == 0 || desc->dataSize == 0 || desc->indexCount <= 0 || desc->vertexCount <= 0)
//		return nullptr;
//	if (desc->vertices == 0 || desc->indices == 0)
//		return nullptr;
//	DMesh* mesh = new DMesh();
//	mesh->Init(desc);
//	
//	return mesh;
//}

//void DMesh::Init(DMeshBufferDesc* desc)
//{
//	m_vertexBuffer = desc->vertices;
//	m_indexBuffer = desc->indices;
//	m_vertexCount = desc->vertexCount;
//	m_indexCount = desc->indexCount;
//	m_dataSize = desc->dataSize;
//	m_bufferLength = desc->dataCount;
//	m_vertexOffset = 8;
//	m_uvOffset = 3;
//	m_normalOffset = 5;
//
//	m_meshRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateMeshRes();
//	m_meshRes->Init(desc);
//
//	delete[] desc->indices;
//	delete[] desc->vertices;
//
//	desc->indices = 0;
//	desc->vertices = 0;
//}

void DMesh::UpdateMeshReses()
{
	std::map<int, DMeshRes*>::iterator  iter;
	for (iter = m_meshReses.begin(); iter != m_meshReses.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Refresh(&m_meshDesc);
		}
	}
}

//void DMesh::MarkMeshChanged()
//{
//}

