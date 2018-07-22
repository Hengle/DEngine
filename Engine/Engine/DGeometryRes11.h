#pragma once
#ifdef _DGAPI_D3D11

#include "D3D11Core.h"

/*
	Geometry资源Direct3D11底层
*/
class DGeometryRes11 : public DGeometryRes
{
public:
	DGeometryRes11(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int vertexUsage /*顶点用法*/, bool dynamic);
	~DGeometryRes11();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
};

#endif