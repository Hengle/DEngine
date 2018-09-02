#include "DGeometry.h"
#include "DModelLoader.h"
#include "DSystem.h"


DGeometry::DGeometry(bool dynamic)
{
	m_boneMatrices = 0;
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

	m_isBoundsChanged = true;
}


DGeometry::~DGeometry()
{
}

void DGeometry::Destroy()
{
	std::map<int, DGeometryWrapper*>::iterator  iter;
	for (iter = m_geometryWrappers.begin(); iter != m_geometryWrappers.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
			delete iter->second;
		}
	}
	m_geometryWrappers.clear();

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
	if (m_geometryDesc.tangents != NULL)
	{
		delete[] m_geometryDesc.tangents;
		m_geometryDesc.tangents = 0;
	}
	if (m_boneMatrices != NULL)
	{
		delete[] m_boneMatrices;
		m_boneMatrices = 0;
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

void DGeometry::GetTangent(int index, DVector4 * tangent) const
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (m_geometryDesc.tangents == 0)
		return;
	float x = m_geometryDesc.tangents[index * 4];
	float y = m_geometryDesc.tangents[index * 4 + 1];
	float z = m_geometryDesc.tangents[index * 4 + 2];
	float w = m_geometryDesc.tangents[index * 4 + 3];
	tangent->x = x;
	tangent->y = y;
	tangent->z = z;
	tangent->w = w;
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

void DGeometry::GetBoundsRange(DVector3 * center, DVector3 * size)
{
	if (m_isBoundsChanged)
	{
		UpdateBounds();
		m_isBoundsChanged = false;
	}
	*center = m_centerVertex;
	*size = m_rangeSize;
}

bool DGeometry::IsBoundsRangeChanged()
{
	return m_isBoundsChanged;
}

DGeometryTopology DGeometry::GetTopology()
{
	return m_topology;
}

void DGeometry::SetVertex(int index, const DVector3 & vertex)
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (m_geometryDesc.vertices == 0)
		return;
	m_geometryDesc.vertices[index*3] = vertex.x;
	m_geometryDesc.vertices[index*3 + 1] = vertex.y;
	m_geometryDesc.vertices[index*3 + 2] = vertex.z;
	m_vertexChanged = true;
	m_isBoundsChanged = true;
}

void DGeometry::SetNormal(int index, const DVector3 & normal)
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (m_geometryDesc.normals == 0)
		return;
	m_geometryDesc.normals[index*3] = normal.x;
	m_geometryDesc.normals[index*3 + 1] = normal.y;
	m_geometryDesc.normals[index*3 + 2] = normal.z;
	m_vertexChanged = true;
}

void DGeometry::SetColor(int index, const DColor & color)
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (m_geometryDesc.colors == 0)
		return;
	m_geometryDesc.colors[index*4] = color.r;
	m_geometryDesc.colors[index*4 + 1] = color.g;
	m_geometryDesc.colors[index*4 + 2] = color.b;
	m_geometryDesc.colors[index*4 + 3] = color.a;
	m_vertexChanged = true;
}

void DGeometry::SetVertices(DVector3 * vertices, int length)
{
	unsigned int i;
	if (m_geometryDesc.vertices != 0)
	{
		delete[] m_geometryDesc.vertices;
		m_geometryDesc.vertices = 0;
	}
	m_geometryDesc.vertices = new float[length * 3];
	
	for (i = 0; i < length; i++)
	{
		m_geometryDesc.vertices[i * 3] = vertices[i].x;
		m_geometryDesc.vertices[i * 3 + 1] = vertices[i].y;
		m_geometryDesc.vertices[i * 3 + 2] = vertices[i].z;

	}
	
	m_isBoundsChanged = true;
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
	m_isBoundsChanged = true;
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

void DGeometry::SetIndices(unsigned int * indices, int length)
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

void DGeometry::SetBoneMatrixCount(int count)
{
	if (m_boneMatrices != 0)
	{
		delete[] m_boneMatrices;
		m_boneMatrices = 0;
	}
	m_boneMatrices = new DMatrix4x4[count];
	m_boneMatrixCount = count;
}

void DGeometry::SetBoneMatrix(DMatrix4x4 matrix, int index)
{
	if (index < 0 || index >= m_boneMatrixCount)
	{
		return;
	}
	m_boneMatrices[index] = matrix;
}

void DGeometry::SetUV(int index, int channel, const DVector2 & uv)
{
	if (index < 0 || index >= m_geometryDesc.vertexCount)
		return;
	if (channel == 0)
	{
		if (m_geometryDesc.uvs == 0)
			return;
		m_geometryDesc.uvs[index*2] = uv.x;
		m_geometryDesc.uvs[index*2 + 1] = uv.y;
		m_vertexChanged = true;
	}
	else if (channel == 1)
	{
		if (m_geometryDesc.uv2s == 0)
			return;
		m_geometryDesc.uv2s[index * 2] = uv.x;
		m_geometryDesc.uv2s[index * 2 + 1] = uv.y;
		m_vertexChanged = true;
	}
	else if (channel == 2)
	{
		if (m_geometryDesc.uv3s == 0)
			return;
		m_geometryDesc.uv3s[index * 2] = uv.x;
		m_geometryDesc.uv3s[index * 2 + 1] = uv.y;
		m_vertexChanged = true;
	}
}

void DGeometry::SetTopology(DGeometryTopology topology)
{
	m_topology = topology;
}

void DGeometry::GetIndex(int index, unsigned int & outIndex) const
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
	if (vertexUsage < 0)
		return;
	
	DGraphics::ApplyActiveMaterial();

	if (m_geometryWrappers.find(vertexUsage) == m_geometryWrappers.end())
	{
		DGeometryWrapper* wrapper = DSystem::GetGraphicsMgr()->GetGLCore()->CreateGeometryWrapper(vertexUsage, m_dynamic);
		wrapper->SetDirty();
	
		m_geometryWrappers.insert(std::pair<int, DGeometryWrapper*>(vertexUsage, wrapper));
	}
	if (m_vertexChanged)
	{
		UpdateGeometryReses();
		m_vertexChanged = false;
	}
	if (m_geometryWrappers.find(vertexUsage) != m_geometryWrappers.end())
	{
		DGeometryWrapper* wrapper = m_geometryWrappers.at(vertexUsage);
		if (wrapper->IsDirty())
		{
			wrapper->Refresh(&m_geometryDesc);
		}
		wrapper->DrawPrimitive(m_topology);
	}
}

void DGeometry::UpdateBone(DTransform ** bones, const DMatrix4x4& worldToLocal)
{
	if (m_geometryDesc.boneIndices == 0 || m_geometryDesc.boneWeights == 0 || m_boneMatrices == 0)
		return;
	int i;
	DTransform* bone0, *bone1, *bone2, *bone3;
	int boneIndex0, boneIndex1, boneIndex2, boneIndex3;
	float weight0, weight1, weight2, weight3;
	DMatrix4x4 pose0, pose1, pose2, pose3;
	DMatrix4x4 boneMat0, boneMat1, boneMat2, boneMat3;

	DVector3 pos,result,temp;
	
	for (i = 0; i < m_geometryDesc.vertexCount; i++)
	{
		boneIndex0 = m_geometryDesc.boneIndices[i * 4];
		boneIndex1 = m_geometryDesc.boneIndices[i * 4 + 1];
		boneIndex2 = m_geometryDesc.boneIndices[i * 4 + 2];
		boneIndex3 = m_geometryDesc.boneIndices[i * 4 + 3];

		bone0 = bones[boneIndex0];
		bone1 = bones[boneIndex1];
		bone2 = bones[boneIndex2];
		bone3 = bones[boneIndex3];

		pose0 = m_boneMatrices[boneIndex0];
		pose1 = m_boneMatrices[boneIndex1];
		pose2 = m_boneMatrices[boneIndex2];
		pose3 = m_boneMatrices[boneIndex3];

		weight0 = m_geometryDesc.boneWeights[i * 4];
		weight1 = m_geometryDesc.boneWeights[i * 4 + 1];
		weight2 = m_geometryDesc.boneWeights[i * 4 + 2];
		weight3 = m_geometryDesc.boneWeights[i * 4 + 3];

		pos = DVector3(m_geometryDesc.vertices[i * 3], m_geometryDesc.vertices[i * 3 + 1], m_geometryDesc.vertices[i * 3 + 2]);

		bone0->GetLocalToWorld(boneMat0);
		bone1->GetLocalToWorld(boneMat1);
		bone2->GetLocalToWorld(boneMat2);
		bone3->GetLocalToWorld(boneMat3);

		(pose0*boneMat0*worldToLocal).TransformPoint(pos, temp);
		result += temp*weight0;
		
		(pose1*boneMat1*worldToLocal).TransformPoint(pos, temp);
		result += temp*weight1;

		(pose2*boneMat2*worldToLocal).TransformPoint(pos, temp);
		result += temp*weight2;

		(pose3*boneMat3*worldToLocal).TransformPoint(pos, temp);
		result += temp*weight3;
	}
	m_vertexChanged = true;
}

DGeometry * DGeometry::Create(char* fileName, bool dynamic)
{
	DGeometryBufferDesc desc;
	int len = strlen(fileName);
	//TODO 临时处理
	if (fileName[len - 3] == 'o' && fileName[len - 2] == 'b'&&fileName[len - 1] == 'j')
		DModelLoader::LoadObj(fileName, &desc);
	else if (fileName[len - 4] == 'm' && fileName[len - 3] == 'e' && fileName[len - 2] == 's' && fileName[len - 1] == 'h')
		DModelLoader::LoadMesh(fileName, &desc);
	else
		return NULL;

	DGeometry* geometry = new DGeometry(dynamic);
	geometry->m_geometryDesc = desc;

	return geometry;
}

void DGeometry::UpdateGeometryReses()
{
	std::map<int, DGeometryWrapper*>::iterator  iter;
	for (iter = m_geometryWrappers.begin(); iter != m_geometryWrappers.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->SetDirty();
			//iter->second->Refresh(&m_geometryDesc);
		}
	}
}

void DGeometry::UpdateBounds()
{
	int len = m_geometryDesc.vertexCount;
	int i;
	float maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY, minX = INFINITY, minY = INFINITY, minZ = INFINITY;
	for (i = 0; i < len; i++)
	{
		if (maxX < m_geometryDesc.vertices[i * 3])
			maxX = m_geometryDesc.vertices[i * 3];
		if (maxY < m_geometryDesc.vertices[i * 3 + 1])
			maxY = m_geometryDesc.vertices[i * 3 + 1];
		if (maxZ < m_geometryDesc.vertices[i * 3 + 2])
			maxZ = m_geometryDesc.vertices[i * 3 + 2];

		if (minX > m_geometryDesc.vertices[i * 3])
			minX = m_geometryDesc.vertices[i * 3];
		if (minY > m_geometryDesc.vertices[i * 3 + 1])
			minY = m_geometryDesc.vertices[i * 3 + 1];
		if (minZ > m_geometryDesc.vertices[i * 3 + 2])
			minZ = m_geometryDesc.vertices[i * 3 + 2];
	}
	
	m_rangeSize = DVector3(maxX - minX, maxY - minY, maxZ - minZ);
	m_centerVertex = DVector3(minX + m_rangeSize.x * 0.5f, minY + m_rangeSize.y * 0.5f, minZ + m_rangeSize.z * 0.5f);
}

