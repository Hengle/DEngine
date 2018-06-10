#pragma once
#include "D3D11Core.h"

class DMeshRes11 : public DMeshRes
{
public:
	DMeshRes11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DMeshRes11();
	virtual void Release();

protected:
	virtual bool OnInit(DMeshBufferDesc*);
	virtual void OnDraw();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	int m_dataSize;
	int m_indexCount;
};

