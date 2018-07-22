#pragma once
#ifdef _DGAPI_D3D9
#include "D3D9Core.h"

/*
	Geometry资源Direct3D9底层
*/
class DGeometryRes9 : public DGeometryRes
{
public:
	DGeometryRes9(LPDIRECT3DDEVICE9, int vertexUsage /*顶点用法*/, bool dynamic);
	~DGeometryRes9();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);

private:
	LPDIRECT3DDEVICE9 m_device;
	IDirect3DVertexBuffer9* m_vertexBuffer;
	IDirect3DIndexBuffer9* m_indexBuffer;
	IDirect3DVertexDeclaration9* m_vertexDeclaration;
};

#endif