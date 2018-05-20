#pragma once
#include <d3d11.h>
#include <d3dx10math.h>

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

protected:
	virtual void CreateBuffer(void**, unsigned long **, int&, int&, int&) = 0;

protected:
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;

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

class DSphereMesh : public DMesh
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 tex;
		D3DXVECTOR3 normal;
	};
public:
	DSphereMesh();
	DSphereMesh(float);
	~DSphereMesh();

protected:
	virtual void CreateBuffer(void**, unsigned long**, int&, int&, int&);

private:
	float m_radius;
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