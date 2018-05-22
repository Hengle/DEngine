#pragma once
#include "DResObject.h"
#include <D3DX11tex.h>

class DTexture : public DResObject
{
public:
	DTexture();
	~DTexture();
	virtual bool Init(ID3D11Device*) = 0;

};

class DTexture2D : public DTexture 
{
public:
	DTexture2D(WCHAR*);
	~DTexture2D();
	virtual void Destroy();
	virtual bool Init(ID3D11Device*);
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
	WCHAR* m_textureFile;
};

class DTextureCube : public DTexture
{

};

