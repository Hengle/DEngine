#pragma once
#ifdef _DGAPI_D3D9
#include "D3DCore.h"
#include "D3D9Core.h"

/*
	Geometry包装器Direct3D9底层
*/
class DGeometryWrapper9 : public DGeometryWrapper
{
public:
	DGeometryWrapper9(LPDIRECT3DDEVICE9, int vertexUsage /*顶点用法*/, bool dynamic);
	~DGeometryWrapper9();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);

private:
	LPDIRECT3DDEVICE9 m_device;
	IDirect3DVertexBuffer9* m_vertexBuffer;
	IDirect3DIndexBuffer9* m_indexBuffer;
	IDirect3DVertexDeclaration9* m_vertexDeclaration;
};

#endif