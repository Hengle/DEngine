#pragma once
#include "D3D11Core.h"

class DTextureRes11 : public DTextureRes
{
public:
	DTextureRes11(ID3D11Device* device, ID3D11DeviceContext * deviceContext, WCHAR* filename);
	~DTextureRes11();
	virtual void Apply(UINT, DWrapMode);
	virtual void Release();

private:
	ID3D11ShaderResourceView* m_texture;
	bool m_isSuccess;
	ID3D11DeviceContext* m_deviceContext;
};

class DRenderTextureViewRes : public DRenderTextureViewRes
{

};

