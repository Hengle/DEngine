#pragma once
#include "DMath.h"
#include "DColor.h"
#include "DGLCore.h"
#include "DResObject.h"
#include <map>

enum DMeshDefine
{
	DMESH_Cube = 0,
	DMESH_Sphere = 1,
	DMESH_Plane = 2,
};

class DMesh : public DResObject
{

public:
	DMesh(bool = false);
	~DMesh();
	virtual void Destroy();
	int GetIndexCount() const;
	int GetVertexCount() const;
	void GetVertex(int index, DVector3* vertex) const;
	void GetNormal(int index, DVector3* normal) const;
	void GetColor(int index, DColor* color) const;
	void GetUV(int index, int channel, DVector2* uv) const;
	DMeshTopology GetTopology();
	void SetVertex(int index, const DVector3&);
	void SetNormal(int index, const DVector3&);
	void SetColor(int index, const DColor&);
	void SetUV(int index, int channel, const DVector2&);
	void SetVertices(DVector3*, int);
	void SetVertices(float*, int);
	void SetUVs(int channel, DVector2*, int);
	void SetUVs(int channel, float*, int);
	void SetNormals(DVector3*, int);
	void SetColors(DColor*, int);
	void SetIndices(unsigned long*, int);
	void SetTopology(DMeshTopology);
	void GetIndex(int index, unsigned long& outIndex) const;
	bool HasNormal() const;
	bool HasColor() const;
	bool HasUV(int channel) const;
	void Draw(int);

	static DMesh* Create(DMeshDefine meshDefine, bool dynamic = false);
	static DMesh*Create(char* fileName, bool dynamic = false);
	
private:
	//static DMesh* Create(DMeshBufferDesc*);
	//void Init(DMeshBufferDesc*);
	void UpdateMeshReses();

private:
	bool m_vertexChanged;
	//float* m_vertexBuffer;
	//unsigned long *m_indexBuffer;
	//int m_vertexCount, m_indexCount;
	//int m_dataSize;
	//int m_bufferLength;
	//int m_vertexOffset;
	//int m_normalOffset;
	//int m_colorOffset;
	//int m_uvOffset;
	/*float*m_vertices;
	float*m_uvs;
	float*m_uv2s;
	float*m_uv3s;
	float*m_normals;
	float*m_colors;
	unsigned long*m_indices;*/
	DMeshBufferDesc m_meshDesc;
	DMeshTopology m_topology;
	std::map<int, DMeshRes*> m_meshReses;
	bool m_dynamic;
	//DMeshRes* m_meshRes;
};