#pragma once
#include "DResObject.h"
#include "DGraphics.h"
#include <windows.h>

class DTexture : public DResObject
{
public:
	DTexture();
	~DTexture();
};

class DTexture2D : public DTexture 
{
private:
	DTexture2D();
public:
	~DTexture2D();
	virtual void Destroy();
	DTextureBuffer* GetTextureBuffer();

	static DTexture2D* Create(WCHAR*);

private:
	DTextureBuffer* m_texture;
};

class DTextureCube : public DTexture
{

};

