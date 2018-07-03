#pragma once
#include "D3D11Core.h"

/*
	Mesh资源Direct3D11底层
*/
class DMeshRes11 : public DMeshRes
{
public:
	DMeshRes11(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int vertexUsage, bool dynamic);
	~DMeshRes11();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DMeshTopology);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
};

