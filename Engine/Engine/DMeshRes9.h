#pragma once
#include "D3D9Core.h"

class DMeshRes9 : public DMeshRes
{
public:
	DMeshRes9(LPDIRECT3DDEVICE9);
	~DMeshRes9();
	virtual void Release();

protected:
	virtual bool OnInit(DMeshBufferDesc*);
	virtual void OnDraw(DMeshTopology);

private:
	LPDIRECT3DDEVICE9 m_device;
	ID3DXMesh* m_mesh;
	int m_dataSize;
	int m_vertexCount;
	int m_indexCount;
	int m_pimCount;
};

