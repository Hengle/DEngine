#include "DGeometry.h"
#include "DModelLoader.h"
#include "DSystem.h"

DGeometry::DGeometry(bool dynamic)
{

	m_vertexChanged = false;
	m_geometryDesc.vertices = 0;
	m_geometryDesc.uvs = 0;
	m_geometryDesc.uv2s = 0;
	m_geometryDesc.uv3s = 0;
	m_geometryDesc.normals = 0;
	m_geometryDesc.colors = 0;
	m_geometryDesc.indices = 0;

	m_topology = DGeometryTopology_TriangleList;
	m_dynamic = dynamic;
}


DGeometry::~DGeometry()
{
}

void DGeometry::Destroy()
{
	std::map<int, DGeometryRes*>::iterator  iter;
	for (iter = m_geometryReses.begin(); iter != m_geometryReses.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
			delete iter->second;
		}
	}
	m_geometryReses.clear();

	if (m_geometryDesc.vertices != NULL)
	{
		delete[] m_geometryDesc.vertices;
		m_geometryDesc.vertices = 0;
	}
	if (m_geometryDesc.uvs != NULL)
	{
		delete[] m_geometryDesc.uvs;
		m_geometryDesc.uvs = 0;
	}
	if (m_geometryDesc.uv2s != NULL)
	{
		delete[] m_geometryDesc.uv2s;
		m_geometryDesc.uv2s = 0;
	}
	if (m_geometryDesc.uv3s != NULL)
	{
		delete[] m_geometryDesc.uv3s;
		m_geometryDesc.uv3s = 0;
	}
	if (m_geometryDesc.normals != NULL)
	{
		delete[] m_geometryDesc.normals;
		m_geometryDesc.normals = 0;
	}
	if (m_geometryDesc.colors != NULL)
	{
		delete[] m_geometryDesc.colors;
		m_geometryDesc.colors = 0;
	}
	if (m_geometryDesc.indices != NULL)
	{
		delete[] m_geometryDesc.indices;
		m_geometryDesc.indices = 0;
	}
}

int DGeometry::GetIndexCount() const
{
	return m_geometryDesc.indexCount;
}

int DGeometry::GetVertexCount() const
{
	return m_geometryDesc.vertexCount;
}

void DGeometry::GetVertex(int index, DVector3 * vertex) const
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (m_geometryDesc.vertices == 0)
		return;
	float x = m_geometryDesc.vertices[index*3];
	float y = m_geometryDesc.vertices[index*3 + 1];
	float z = m_geometryDesc.vertices[index*3 + 2];
	vertex->x = x;
	vertex->y = y;
	vertex->z = z;
}

void DGeometry::GetNormal(int index, DVector3 * normal) const
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (m_geometryDesc.normals == 0)
		return;
	float x = m_geometryDesc.normals[index*3];
	float y = m_geometryDesc.normals[index*3 + 1];
	float z = m_geometryDesc.normals[index*3 + 2];
	normal->x = x;
	normal->y = y;
	normal->z = z;
}

void DGeometry::GetColor(int index, DColor * color) const
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (m_geometryDesc.colors == 0)
		return;
	float r = m_geometryDesc.colors[index*4];
	float g = m_geometryDesc.colors[index*4 + 1];
	float b = m_geometryDesc.colors[index*4 + 2];
	float a = m_geometryDesc.colors[index*4 + 3];
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;
}

void DGeometry::GetUV(int index, int channel, DVector2 * uv) const
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (channel == 0)
	{
		if (m_geometryDesc.uvs == 0)
			return;
		uv->x = m_geometryDesc.uvs[index*2];
		uv->y = m_geometryDesc.uvs[index*2+1];
	}
	else if (channel == 1)
	{
		if (m_geometryDesc.uv2s == 0)
			return;
		uv->x = m_geometryDesc.uv2s[index * 2];
		uv->y = m_geometryDesc.uv2s[index * 2 + 1];
	}
	else if (channel == 1)
	{
		if (m_geometryDesc.uv3s == 0)
			return;
		uv->x = m_geometryDesc.uv3s[index * 2];
		uv->y = m_geometryDesc.uv3s[index * 2 + 1];
	}
}

void DGeometry::GetBounds(DBounds * bounds)
{
	*bounds = m_Bounds;
}

DGeometryTopology DGeometry::GetTopology()
{
	return m_topology;
}

//void DGeometry::SetVertex(int index, const DVector3 & vertex)
//{
//	if (index < 0 || index >= m_geometryDesc.vertexCount)
//		return;
//	if (m_geometryDesc.vertices == 0)
//		return;
//	m_geometryDesc.vertices[index*3] = vertex.x;
//	m_geometryDesc.vertices[index*3 + 1] = vertex.y;
//	m_geometryDesc.vertices[index*3 + 2] = vertex.z;
//	m_vertexChanged = true;
//}
//
//void DGeometry::SetNormal(int index, const DVector3 & normal)
//{
//	if (index < 0 || index >= m_geometryDesc.vertexCount)
//		return;
//	if (m_geometryDesc.normals == 0)
//		return;
//	m_geometryDesc.normals[index*3] = normal.x;
//	m_geometryDesc.normals[index*3 + 1] = normal.y;
//	m_geometryDesc.normals[index*3 + 2] = normal.z;
//	m_vertexChanged = true;
//}
//
//void DGeometry::SetColor(int index, const DColor & color)
//{
//	if (index < 0 || index >= m_geometryDesc.vertexCount)
//		return;
//	if (m_geometryDesc.colors == 0)
//		return;
//	m_geometryDesc.colors[index*4] = color.r;
//	m_geometryDesc.colors[index*4 + 1] = color.g;
//	m_geometryDesc.colors[index*4 + 2] = color.b;
//	m_geometryDesc.colors[index*4 + 3] = color.a;
//	m_vertexChanged = true;
//}

void DGeometry::SetVertices(DVector3 * vertices, int length)
{
	unsigned int i;
	if (m_geometryDesc.vertices != 0)
	{
		delete[] m_geometryDesc.vertices;
		m_geometryDesc.vertices = 0;
	}
	m_geometryDesc.vertices = new float[length * 3];
	float maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY, minX = INFINITY, minY = INFINITY, minZ = INFINITY;
	for (i = 0; i < length; i++)
	{
		m_geometryDesc.vertices[i * 3] = vertices[i].x;
		m_geometryDesc.vertices[i * 3 + 1] = vertices[i].y;
		m_geometryDesc.vertices[i * 3 + 2] = vertices[i].z;

		if (maxX < vertices[i].x)
			maxX = vertices[i].x;
		if (maxY < vertices[i].y)
			maxY = vertices[i].y;
		if (maxZ < vertices[i].z)
			maxZ = vertices[i].z;

		if (minX > vertices[i].x)
			minX = vertices[i].x;
		if (minY > vertices[i].y)
			minY = vertices[i].y;
		if (minZ > vertices[i].z)
			minZ = vertices[i].z;
	}
	m_geometryDesc.vertexCount = length;
	m_vertexChanged = true;
}

void DGeometry::SetVertices(float * vertices, int vertexCount)
{
	if (m_geometryDesc.vertices != 0)
	{
		delete[] m_geometryDesc.vertices;
		m_geometryDesc.vertices = 0;
	}
	m_geometryDesc.vertices = vertices;
	m_geometryDesc.vertexCount = vertexCount;
	m_vertexChanged = true;
}

void DGeometry::SetUVs(int channel, DVector2 * uvs, int length)
{
	if (m_geometryDesc.vertexCount != length)
		return;
	if (channel == 0)
	{
		unsigned int i;
		if (m_geometryDesc.uvs != 0)
		{
			delete[] m_geometryDesc.uvs;
			m_geometryDesc.uvs = 0;
		}
		m_geometryDesc.uvs = new float[length * 2];
		for (i = 0; i < length; i++)
		{
			m_geometryDesc.uvs[i * 3] = uvs[i].x;
			m_geometryDesc.uvs[i * 3 + 1] = uvs[i].y;
		}
		m_vertexChanged = true;
	}
	else if (channel == 1)
	{
		unsigned int i;
		if (m_geometryDesc.uv2s != 0)
		{
			delete[] m_geometryDesc.uv2s;
			m_geometryDesc.uv2s = 0;
		}
		m_geometryDesc.uv2s = new float[length * 2];
		for (i = 0; i < length; i++)
		{
			m_geometryDesc.uv2s[i * 3] = uvs[i].x;
			m_geometryDesc.uv2s[i * 3 + 1] = uvs[i].y;
		}
		m_vertexChanged = true;
	}
}

void DGeometry::SetUVs(int channel, float * uvs, int length)
{
	if (m_geometryDesc.vertexCount != length)
		return;
	if (channel == 0)
	{
		unsigned int i;
		if (m_geometryDesc.uvs != 0)
		{
			delete[] m_geometryDesc.uvs;
			m_geometryDesc.uvs = 0;
		}
		m_geometryDesc.uvs = uvs;
		m_vertexChanged = true;
	}
}

void DGeometry::SetNormals(DVector3 *normals, int length)
{
	if (m_geometryDesc.vertexCount != length)
		return;
	unsigned int i;
	if (m_geometryDesc.normals != 0)
	{
		delete[] m_geometryDesc.normals;
		m_geometryDesc.normals = 0;
	}
	m_geometryDesc.normals = new float[length * 3];
	for (i = 0; i < length; i++)
	{
		m_geometryDesc.normals[i * 3] = normals[i].x;
		m_geometryDesc.normals[i * 3 + 1] = normals[i].y;
		m_geometryDesc.normals[i * 3 + 2] = normals[i].z;
	}
	m_vertexChanged = true;
}

void DGeometry::SetColors(DColor * colors, int length)
{
	if (m_geometryDesc.vertexCount != length)
		return;
	unsigned int i;
	if (m_geometryDesc.colors != 0)
	{
		delete[] m_geometryDesc.colors;
		m_geometryDesc.colors = 0;
	}
	m_geometryDesc.colors = new float[length * 4];
	for (i = 0; i < length; i++)
	{
		m_geometryDesc.colors[i * 4] = colors[i].r;
		m_geometryDesc.colors[i * 4 + 1] = colors[i].g;
		m_geometryDesc.colors[i * 4 + 2] = colors[i].b;
		m_geometryDesc.colors[i * 4 + 3] = colors[i].a;
	}
	m_vertexChanged = true;
}

void DGeometry::SetIndices(unsigned long * indices, int length)
{
	if (m_geometryDesc.indices != 0)
	{
		delete[] m_geometryDesc.indices;
		m_geometryDesc.indices = 0;
	}
	m_geometryDesc.indices = indices;
	m_geometryDesc.indexCount = length;
	m_vertexChanged = true;
}

//void DGeometry::SetUV(int index, int channel, const DVector2 & uv)
//{
//	if (index < 0 || index >= m_geometryDesc.vertexCount)
//		return;
//	if (channel == 0)
//	{
//		if (m_geometryDesc.uvs == 0)
//			return;
//		m_geometryDesc.uvs[index*2] = uv.x;
//		m_geometryDesc.uvs[index*2 + 1] = uv.y;
//		m_vertexChanged = true;
//	}
//	else if (channel == 1)
//	{
//		if (m_geometryDesc.uv2s == 0)
//			return;
//		m_geometryDesc.uv2s[index * 2] = uv.x;
//		m_geometryDesc.uv2s[index * 2 + 1] = uv.y;
//		m_vertexChanged = true;
//	}
//	else if (channel == 2)
//	{
//		if (m_geometryDesc.uv3s == 0)
//			return;
//		m_geometryDesc.uv3s[index * 2] = uv.x;
//		m_geometryDesc.uv3s[index * 2 + 1] = uv.y;
//		m_vertexChanged = true;
//	}
//}

void DGeometry::SetTopology(DGeometryTopology topology)
{
	m_topology = topology;
}

void DGeometry::GetIndex(int index, unsigned long & outIndex) const
{
	if (index < 0 || index >= m_geometryDesc.indexCount)
		return;
	outIndex = m_geometryDesc.indices[index];
}

bool DGeometry::HasNormal() const
{
	return m_geometryDesc.normals != 0;
}

bool DGeometry::HasColor() const
{
	return m_geometryDesc.colors != 0;
}

bool DGeometry::HasUV(int channel) const
{
	if (channel == 0)
		return m_geometryDesc.uvs != 0;
	if (channel == 1)
		return m_geometryDesc.uv2s != 0;
	if (channel == 2)
		return m_geometryDesc.uv3s != 0;
	return false;
}

void DGeometry::Draw(int vertexUsage)
{
	if (vertexUsage <= 0)
		return;
	
	DGraphics::ApplyActiveMaterial();

	if (m_geometryReses.find(vertexUsage) == m_geometryReses.end())
	{
		DGeometryRes* res = DSystem::GetGraphicsMgr()->GetGLCore()->CreateGeometryRes(vertexUsage, m_dynamic);
	
		m_geometryReses.insert(std::pair<int, DGeometryRes*>(vertexUsage, res));
	}
	if (m_vertexChanged)
	{
		UpdateGeometryReses();
		m_vertexChanged = false;
	}
	if (m_geometryReses.find(vertexUsage) != m_geometryReses.end())
	{
		DGeometryRes* res = m_geometryReses.at(vertexUsage);
		if (!res->IsInitialized())
		{
			res->Refresh(&m_geometryDesc);
		}
		res->DrawPrimitive(m_topology);
	}
}

DGeometry * DGeometry::Create(DGeometryDefine geometryDefine, bool dynamic)
{
	DGeometryBufferDesc desc;
	if (geometryDefine == DGeometry_Plane)
	{
		DModelLoader::CreatePlane(&desc);
	}
	else if (geometryDefine == DGeometry_Cube)
	{
		DModelLoader::CreateCube(&desc);
	}
	else
		return NULL;

	DGeometry* geometry = new DGeometry(dynamic);
	geometry->m_geometryDesc = desc;

	return geometry;
}

DGeometry * DGeometry::Create(char* fileName, bool dynamic)
{
	DGeometryBufferDesc desc;
	DModelLoader::LoadObj(fileName, &desc);

	DGeometry* geometry = new DGeometry(dynamic);
	geometry->m_geometryDesc = desc;

	return geometry;
}

void DGeometry::UpdateGeometryReses()
{
	std::map<int, DGeometryRes*>::iterator  iter;
	for (iter = m_geometryReses.begin(); iter != m_geometryReses.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Refresh(&m_geometryDesc);
		}
	}
}

