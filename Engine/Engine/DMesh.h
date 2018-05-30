#pragma once
#include "DMath.h"
#include "DColor.h"
#include "DGLCore.h"

enum DMeshDefine
{
	Cube,
	Sphere,
	Plane,
};

class DMesh
{
private:
	DMesh();

public:
	~DMesh();
	void Destroy();
	int GetIndexCount();
	int GetVertexCount();
	void GetVertex(int index, DVector3* vertex);
	void GetNormal(int index, DVector3* normal);
	void GetColor(int index, DColor* color);
	void GetUV(int index, int channel, DVector2* uv);
	void SetVertex(int index, const DVector3&);
	void SetNormal(int index, const DVector3&);
	void SetColor(int index, const DColor&);
	void SetUV(int index, int channel, const DVector3&);
	void GetIndex(int index, unsigned long& outIndex);
	bool HasNormal();
	bool HasColor();
	bool HasUV(int channel);
	int GetDataSize();
	DMeshBuffer* GetBuffer();

	static DMesh* Create(DMeshDefine meshDefine);
	static DMesh*Create(char* fileName);

private:
	DMeshBuffer* m_meshBuffer;
	bool m_vertexChanged;
	float* m_vertexBuffer;
	unsigned long *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_dataSize;
	int m_vertexOffset;
	int m_normalOffset;
	int m_colorOffset;
	int m_uvOffset;
};