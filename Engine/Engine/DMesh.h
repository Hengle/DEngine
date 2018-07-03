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
	void UpdateMeshReses();

private:
	bool m_vertexChanged;
	DMeshBufferDesc m_meshDesc;
	DMeshTopology m_topology;
	std::map<int, DMeshRes*> m_meshReses;
	bool m_dynamic;
};