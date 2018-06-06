#pragma once
#include "DMath.h"
#include "DColor.h"
#include "DGLCore.h"

enum DMeshDefine
{
	DMESH_Cube = 0,
	DMESH_Sphere = 1,
	DMESH_Plane = 2,
};

class DMesh
{
private:
	DMesh();

public:
	~DMesh();
	void Destroy();
	int GetIndexCount() const;
	int GetVertexCount() const;
	void GetVertex(int index, DVector3* vertex) const;
	void GetNormal(int index, DVector3* normal) const;
	void GetColor(int index, DColor* color) const;
	void GetUV(int index, int channel, DVector2* uv) const;
	void SetVertex(int index, const DVector3&);
	void SetNormal(int index, const DVector3&);
	void SetColor(int index, const DColor&);
	void SetUV(int index, int channel, const DVector3&);
	void GetIndex(int index, unsigned long& outIndex) const;
	bool HasNormal() const;
	bool HasColor() const;
	bool HasUV(int channel) const;
	int GetDataSize() const;
	DMeshBuffer* GetBuffer() const;

	static DMesh* Create(DMeshDefine meshDefine);
	static DMesh*Create(char* fileName);

private:
	DMeshBuffer* m_meshBuffer;
	bool m_vertexChanged;
	float* m_vertexBuffer;
	unsigned long *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_dataSize;
	int m_bufferLength;
	int m_vertexOffset;
	int m_normalOffset;
	int m_colorOffset;
	int m_uvOffset;
};