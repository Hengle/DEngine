#pragma once
#include <d3d11.h>

class DMesh
{
public:
	DMesh();
	~DMesh();
	void Init(ID3D11Device*);
	void Destroy();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	int GetVertexCount();
	D3D11_PRIMITIVE_TOPOLOGY GetTopology();
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY);

protected:
	virtual void CreateBuffer(void**, unsigned long **, int&, int&, int&) = 0;
	int GetDataSize();

protected:
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_topology;

private:
	int m_vertexCount, m_indexCount;
	int m_dataSize;

};

class DCubeMesh : public DMesh
{
public:
	DCubeMesh();
	DCubeMesh(float, float, float);
	~DCubeMesh();

protected:
	virtual void CreateBuffer(void**, unsigned long **, int&, int&, int&);

private:
	float m_sizeX, m_sizeY, m_sizeZ;
};

class DModelMesh : public DMesh
{
public:
	DModelMesh(char*);
	~DModelMesh();

protected:
	virtual void CreateBuffer(void**, unsigned long**, int&, int&, int&);

private:
	char* m_fileName;
};