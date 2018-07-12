#pragma once
#include "D3D10Core.h"

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
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);

private:
	ID3D10Device* m_device;
	ID3D10Buffer* m_vertexBuffer, *m_indexBuffer;
};

