#pragma once
#include "DMath.h"
#include "DColor.h"
#include "DGLCore.h"
#include "DResObject.h"
#include "DTransform.h"
#include <map>

/*几何体类*/
class DGeometry : public DResObject
{

public:
	DGeometry(bool = false);
	~DGeometry();
	virtual void Destroy();
	int GetIndexCount() const;
	int GetVertexCount() const;
	void GetVertex(int index, DVector3* vertex) const;
	void GetNormal(int index, DVector3* normal) const;
	void GetTangent(int index, DVector4* tangent) const;
	void GetColor(int index, DColor* color) const;
	void GetUV(int index, int channel, DVector2* uv) const;
	void GetBoundsRange(DVector3* center, DVector3* size);
	bool IsBoundsRangeChanged();
	DGeometryTopology GetTopology();
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
	void SetIndices(unsigned int*, int);
	void SetBoneMatrixCount(int count);
	void SetBoneMatrix(DMatrix4x4, int index);
	void SetTopology(DGeometryTopology);
	void GetIndex(int index, unsigned int& outIndex) const;
	bool HasNormal() const;
	bool HasColor() const;
	bool HasUV(int channel) const;
	void Draw(int);

	void UpdateBone(DTransform** bones, const DMatrix4x4& worldToLocal);

	static DGeometry*Create(char* fileName, bool dynamic = false);
	
private:
	void UpdateGeometryReses();
	void UpdateBounds();

private:
	bool m_vertexChanged;
	DGeometryBufferDesc m_geometryDesc;
	DGeometryTopology m_topology;
	std::map<int, DGeometryWrapper*> m_geometryWrappers;
	bool m_dynamic;
	DVector3 m_centerVertex;
	DVector3 m_rangeSize;
	//DBoneWeight* m_boneWeights;
	DMatrix4x4* m_boneMatrices;
	int m_boneMatrixCount;

	bool m_isBoundsChanged;
};