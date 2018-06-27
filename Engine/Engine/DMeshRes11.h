#pragma once
#include "D3D11Core.h"

class DMeshRes11 : public DMeshRes
{
public:
	DMeshRes11(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int vertexUsage);
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
	int m_dataSize;
	//int m_dataCount;
	//bool m_hasUV;
	//bool m_hasUV1;
	//bool m_hasUV2;
	//bool m_hasUV3;
	//bool m_hasColor;
	//bool m_hasNormal;
	//bool m_hasTangent;
	//bool m_hasBinormal;
	//int m_indexCount;
};

