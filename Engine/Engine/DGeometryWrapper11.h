﻿#pragma once
#ifdef _DGAPI_D3D11

#include "D3D11Core.h"
#include "D3DCore.h"

/*
	Geometry包装器Direct3D11底层
*/
class DGeometryWrapper11 : public DGeometryWrapper
{
public:
	DGeometryWrapper11(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int vertexUsage /*顶点用法*/, bool dynamic);
	~DGeometryWrapper11();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
};

#endif