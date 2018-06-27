#pragma once
#include "D3D9Core.h"

class DMeshRes9 : public DMeshRes
{
public:
	DMeshRes9(LPDIRECT3DDEVICE9, int);
	~DMeshRes9();
	virtual void Release();

protected:
	virtual bool OnInit(DMeshBufferDesc*);
	virtual void OnRefresh(DMeshBufferDesc*);
	virtual void OnDraw(DMeshTopology);

private:
	LPDIRECT3DDEVICE9 m_device;
	ID3DXMesh* m_mesh;
	/*int m_dataSize;
	int m_vertexCount;
	int m_indexCount;
	int m_pimCount;*/
	//int m_dataSize;
	int m_dataCount;
	bool m_hasUV;
	bool m_hasUV1;
	bool m_hasUV2;
	bool m_hasUV3;
	bool m_hasColor;
	bool m_hasNormal;
	bool m_hasTangent;
	bool m_hasBinormal;
	int m_vertexCount;
	int m_indexCount;
	int m_triCount;
};

