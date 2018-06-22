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
	DWarpMode GetWarpMode();
	//DTextureBuffer* GetTextureBuffer();

	static DTexture2D* Create(WCHAR*);
	static DTexture2D* Create(WCHAR*, DWarpMode);

private:
	DTextureRes* m_textureRes;
	DWarpMode m_warpMode;
};

class DTextureCube : public DTexture
{

};

