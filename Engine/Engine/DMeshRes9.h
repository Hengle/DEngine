#pragma once
#include "D3D9Core.h"

/*
	Mesh资源Direct3D9底层
*/
class DMeshRes9 : public DMeshRes
{
public:
	DMeshRes9(LPDIRECT3DDEVICE9, int vertexUsage, bool dynamic);
	~DMeshRes9();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DMeshTopology);

private:
	LPDIRECT3DDEVICE9 m_device;
	IDirect3DVertexBuffer9* m_vertexBuffer;
	IDirect3DIndexBuffer9* m_indexBuffer;
	IDirect3DVertexDeclaration9* m_vertexDeclaration;
};

