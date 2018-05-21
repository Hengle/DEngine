#include "DMesh.h"
#include "DModel.h"

DMesh::DMesh()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_dataSize = 0;

	m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}


DMesh::~DMesh()
{
}

void DMesh::Init(ID3D11Device * device)
{
	void* vertices;
	unsigned long* indices;

	CreateBuffer(&vertices, &indices, m_vertexCount, m_indexCount, m_dataSize);

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_dataSize * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return;
	}

	if (vertices)
		delete[] vertices;
	vertices = 0;

	if (indices)
		delete[] indices;
	indices = 0;
}

void DMesh::Destroy()
{
	if (m_vertexBuffer != NULL)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	if (m_indexBuffer != NULL)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
}

void DMesh::Render(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	stride = m_dataSize;
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(m_topology);
}

int DMesh::GetIndexCount()
{
	return m_indexCount;
}

int DMesh::GetVertexCount()
{
	return m_vertexCount;
}

D3D11_PRIMITIVE_TOPOLOGY DMesh::GetTopology()
{
	return m_topology;
}

void DMesh::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_topology = topology;
}

int DMesh::GetDataSize()
{
	return m_dataSize;
}

DCubeMesh::DCubeMesh() : DCubeMesh(1.0f,1.0f,1.0f)
{
}

DCubeMesh::DCubeMesh(float sizeX, float sizeY, float sizeZ) : DMesh()
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_sizeZ = sizeZ;
}

DCubeMesh::~DCubeMesh()
{
}

void DCubeMesh::CreateBuffer(void ** vertexBuffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& dataSize)
{
	vertexCount = 24;
	indexCount = 36;

	dataSize = sizeof(D3DXVECTOR3)*3;

	(*vertexBuffer) = new D3DXVECTOR3[vertexCount*3];
	(*indexBuffer) = new unsigned long[indexCount] {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

	float sx = m_sizeX*0.5f;
	float sy = m_sizeY*0.5f;
	float sz = m_sizeZ*0.5f;

	((D3DXVECTOR3*)(*vertexBuffer))[0] = D3DXVECTOR3(-sx, -sy, -sz);
	//(*vertexBuffer)[0].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[2] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[3] = D3DXVECTOR3(-sx, sy, -sz);
	//(*vertexBuffer)[1].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[4] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[5] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[6] = D3DXVECTOR3(sx, sy, -sz);
	//(*vertexBuffer)[2].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[7] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[8] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[9] = D3DXVECTOR3(sx, -sy, -sz);
	//(*vertexBuffer)[3].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[10] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[11] = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[12] = D3DXVECTOR3(-sx, sy, -sz);
	//(*vertexBuffer)[4].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[13] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[14] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[15] = D3DXVECTOR3(-sx, sy, sz);
	//(*vertexBuffer)[5].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[16] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[17] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[18] = D3DXVECTOR3(sx, sy, sz);
	//(*vertexBuffer)[6].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[19] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[20] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[21] = D3DXVECTOR3(sx, sy, -sz);
	//(*vertexBuffer)[7].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[22] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[23] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[24] = D3DXVECTOR3(-sx, sy, sz);
	//(*vertexBuffer)[8].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[25] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[26] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[27] = D3DXVECTOR3(-sx, -sy, sz);
	//(*vertexBuffer)[9].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[28] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[29] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[30] = D3DXVECTOR3(sx, -sy, sz);
	//(*vertexBuffer)[10].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[31] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[32] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[33] = D3DXVECTOR3(sx, sy, sz);
	//(*vertexBuffer)[11].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[34] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[35] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[36] = D3DXVECTOR3(-sx, -sy, sz);
	//(*vertexBuffer)[12].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[37] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[38] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[39] = D3DXVECTOR3(-sx, -sy, -sz);
	//(*vertexBuffer)[13].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[40] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[41] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[42] = D3DXVECTOR3(sx, -sy, -sz);
	//(*vertexBuffer)[14].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[43] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[44] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[45] = D3DXVECTOR3(sx, -sy, sz);
	//(*vertexBuffer)[15].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[46] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[47] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[48] = D3DXVECTOR3(-sx, -sy, sz);
	//(*vertexBuffer)[16].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[49] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[50] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[51] = D3DXVECTOR3(-sx, sy, sz);
	//(*vertexBuffer)[17].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[52] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[53] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[54] = D3DXVECTOR3(-sx, sy, -sz);
	//(*vertexBuffer)[18].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[55] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[56] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[57] = D3DXVECTOR3(-sx, -sy, -sz);
	//(*vertexBuffer)[19].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[58] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[59] = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[60] = D3DXVECTOR3(sx, -sy, -sz);
	//(*vertexBuffer)[20].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[61] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[62] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[63] = D3DXVECTOR3(sx, sy, -sz);
	//(*vertexBuffer)[21].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[64] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[65] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[66] = D3DXVECTOR3(sx, sy, sz);
	//(*vertexBuffer)[22].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[67] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[68] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	((D3DXVECTOR3*)(*vertexBuffer))[69] = D3DXVECTOR3(sx, -sy, sz);
	//(*vertexBuffer)[23].color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[70] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	((D3DXVECTOR3*)(*vertexBuffer))[71] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

DSphereMesh::DSphereMesh() : DSphereMesh(0.5f)
{
}

DSphereMesh::DSphereMesh(float radius) : DMesh()
{
	m_radius = radius;
}

DSphereMesh::~DSphereMesh()
{
}

void DSphereMesh::CreateBuffer(void ** vertexBuffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& dataSize)
{
	vertexCount = 24;
	indexCount = 36;

	dataSize = sizeof(VertexType);

	(*vertexBuffer) = new VertexType[vertexCount];
	(*indexBuffer) = new unsigned long[indexCount] {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
	};

	float sx = 0.5f;
	float sy = 0.5f;
	float sz = 0.5f;

	((VertexType*)(*vertexBuffer))[0].position = D3DXVECTOR3(-sx, -sy, -sz);
	((VertexType*)(*vertexBuffer))[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	((VertexType*)(*vertexBuffer))[1].position = D3DXVECTOR3(-sx, sy, -sz);
	((VertexType*)(*vertexBuffer))[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	((VertexType*)(*vertexBuffer))[2].position = D3DXVECTOR3(sx, sy, -sz);
	((VertexType*)(*vertexBuffer))[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	((VertexType*)(*vertexBuffer))[3].position = D3DXVECTOR3(sx, -sy, -sz);
	((VertexType*)(*vertexBuffer))[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


	((VertexType*)(*vertexBuffer))[4].position = D3DXVECTOR3(-sx, sy, -sz);
	((VertexType*)(*vertexBuffer))[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[4].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[5].position = D3DXVECTOR3(-sx, sy, sz);
	((VertexType*)(*vertexBuffer))[5].tex = D3DXVECTOR2(0.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[5].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[6].position = D3DXVECTOR3(sx, sy, sz);
	((VertexType*)(*vertexBuffer))[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[6].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[7].position = D3DXVECTOR3(sx, sy, -sz);
	((VertexType*)(*vertexBuffer))[7].tex = D3DXVECTOR2(1.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[7].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	((VertexType*)(*vertexBuffer))[8].position = D3DXVECTOR3(-sx, sy, sz);
	((VertexType*)(*vertexBuffer))[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[8].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	((VertexType*)(*vertexBuffer))[9].position = D3DXVECTOR3(-sx, -sy, sz);
	((VertexType*)(*vertexBuffer))[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[9].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	((VertexType*)(*vertexBuffer))[10].position = D3DXVECTOR3(sx, -sy, sz);
	((VertexType*)(*vertexBuffer))[10].tex = D3DXVECTOR2(1.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[10].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	((VertexType*)(*vertexBuffer))[11].position = D3DXVECTOR3(sx, sy, sz);
	((VertexType*)(*vertexBuffer))[11].tex = D3DXVECTOR2(1.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[11].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


	((VertexType*)(*vertexBuffer))[12].position = D3DXVECTOR3(-sx, -sy, sz);
	((VertexType*)(*vertexBuffer))[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[12].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[13].position = D3DXVECTOR3(-sx, -sy, -sz);
	((VertexType*)(*vertexBuffer))[13].tex = D3DXVECTOR2(0.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[13].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[14].position = D3DXVECTOR3(sx, -sy, -sz);
	((VertexType*)(*vertexBuffer))[14].tex = D3DXVECTOR2(1.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[14].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[15].position = D3DXVECTOR3(sx, -sy, sz);
	((VertexType*)(*vertexBuffer))[15].tex = D3DXVECTOR2(1.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[15].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);


	((VertexType*)(*vertexBuffer))[16].position = D3DXVECTOR3(-sx, -sy, sz);
	((VertexType*)(*vertexBuffer))[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[16].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[17].position = D3DXVECTOR3(-sx, sy, sz);
	((VertexType*)(*vertexBuffer))[17].tex = D3DXVECTOR2(0.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[17].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[18].position = D3DXVECTOR3(-sx, sy, -sz);
	((VertexType*)(*vertexBuffer))[18].tex = D3DXVECTOR2(1.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[18].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[19].position = D3DXVECTOR3(-sx, -sy, -sz);
	((VertexType*)(*vertexBuffer))[19].tex = D3DXVECTOR2(1.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[19].normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);


	((VertexType*)(*vertexBuffer))[20].position = D3DXVECTOR3(sx, -sy, -sz);
	((VertexType*)(*vertexBuffer))[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[20].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[21].position = D3DXVECTOR3(sx, sy, -sz);
	((VertexType*)(*vertexBuffer))[21].tex = D3DXVECTOR2(0.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[21].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[22].position = D3DXVECTOR3(sx, sy, sz);
	((VertexType*)(*vertexBuffer))[22].tex = D3DXVECTOR2(1.0f, 1.0f);
	((VertexType*)(*vertexBuffer))[22].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	((VertexType*)(*vertexBuffer))[23].position = D3DXVECTOR3(sx, -sy, sz);
	((VertexType*)(*vertexBuffer))[23].tex = D3DXVECTOR2(1.0f, 0.0f);
	((VertexType*)(*vertexBuffer))[23].normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

DModelMesh::DModelMesh(char * filename) : DMesh()
{
	m_fileName = filename;
}

DModelMesh::~DModelMesh()
{
}

void DModelMesh::CreateBuffer(void ** vertexBuffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& dataSize)
{
	DObjModelLoader::LoadObj(m_fileName, vertexBuffer, indexBuffer, vertexCount, indexCount, dataSize);
}

DLine::DLine() : DMesh()
{
	m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

DLine::~DLine()
{
}

void DLine::UpdateVertex(const D3DXVECTOR3& begin, const D3DXVECTOR3& end, ID3D11DeviceContext * deviceContext)
{
	/*D3DXVECTOR3* vertices = new D3DXVECTOR3[2];
	vertices[0] = begin;
	vertices[1] = end;
	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertices, GetDataSize());
	deviceContext->Unmap(m_vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;*/
}

void DLine::CreateBuffer(void ** vertexBuffer, unsigned long ** indexBuffer, int & vertexCount, int & indexCount, int& dataSize)
{
	vertexCount = 3;
	indexCount = 3;

	dataSize = sizeof(D3DXVECTOR3);

	(*vertexBuffer) = new D3DXVECTOR3[vertexCount];
	((D3DXVECTOR3*)(*vertexBuffer))[0] = D3DXVECTOR3(-5.700611f, -3.575672f, -3.757332f);
	((D3DXVECTOR3*)(*vertexBuffer))[1] = D3DXVECTOR3(-4.700611f, 3.575672f, -4.757332f);
	((D3DXVECTOR3*)(*vertexBuffer))[2] = D3DXVECTOR3(2.700611f, 3.575672f, -4.757332f);
	(*indexBuffer) = new unsigned long[indexCount] {
		0, 1, 2
	};
}
