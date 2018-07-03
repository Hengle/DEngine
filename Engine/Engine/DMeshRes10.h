#pragma once
#include "D3D10Core.h"

/*
	Mesh资源Direct3D10底层
*/
class DMeshRes10 : public DMeshRes
{
public:
	DMeshRes10(ID3D10Device* device, int vertexUsage, bool dynamic);
	~DMeshRes10();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DMeshTopology);

private:
	ID3D10Device* m_device;
	ID3D10Buffer* m_vertexBuffer, *m_indexBuffer;
};

