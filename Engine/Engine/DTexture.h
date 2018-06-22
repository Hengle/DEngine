#pragma once
#include <windows.h>
#include "DGLCore.h"
#include "DResObject.h"

class DTexture : public DResObject
{
public:
	DTexture();
	~DTexture();
	virtual void Apply(UINT);
};

class DTexture2D : public DTexture 
{
private:
	DTexture2D();
public:
	~DTexture2D();
	virtual void Destroy();
	virtual void Apply(UINT);
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

