﻿#pragma once
#ifdef _DGAPI_D3D10

#include "D3D10Core.h"
#include "D3DCore.h"

/*
	Geometry资源Direct3D10底层
*/
class DGeometryRes10 : public DGeometryRes
{
public:
	DGeometryRes10(ID3D10Device* device, int vertexUsage, bool dynamic);
	~DGeometryRes10();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned int* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);

private:
	ID3D10Device* m_device;
	ID3D10Buffer* m_vertexBuffer, *m_indexBuffer;
};

#endif