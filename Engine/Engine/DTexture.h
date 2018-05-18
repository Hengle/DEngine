#pragma once
#include "DResObject.h"
#include <D3DX11tex.h>

class DTexture : public DResObject
{
public:
	DTexture(WCHAR*);
	~DTexture();
	virtual void Destroy();
	bool Init(ID3D11Device*);
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
	WCHAR* m_textureFile;
};

class DTexture2D : public DTexture 
{

};

class DTextureCube : public DTexture
{

};

