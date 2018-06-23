#pragma once
#include "D3D10Core.h"

class DTextureRes10 : public DTextureRes
{
public:
	DTextureRes10(ID3D10Device* device, WCHAR* filename);
	~DTextureRes10();
	virtual void Apply(UINT, UINT, DWarpMode);
	virtual void Release();

private:
	ID3D10ShaderResourceView* m_texture;
	bool m_isSuccess;
	ID3D10Device* m_device;
};

