#pragma once
#include "D3D10Core.h"

class DMeshRes10 : public DMeshRes
{
public:
	DMeshRes10(ID3D10Device* device, int vertexUsage);
	~DMeshRes10();
	virtual void Release();

protected:
	virtual bool OnInit(DMeshBufferDesc*);
	virtual void OnRefresh(DMeshBufferDesc*);
	virtual void OnDraw(DMeshTopology);

private:
	ID3D10Device* m_device;
	ID3D10Buffer* m_vertexBuffer, *m_indexBuffer;
	int m_dataSize, m_indexCount;
};

