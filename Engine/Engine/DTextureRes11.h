#pragma once
#include "D3D11Core.h"

class DTextureRes11 : public DTextureRes
{
public:
	DTextureRes11(ID3D11DeviceContext* deviceContext);
	~DTextureRes11();
	virtual void Release();
};

