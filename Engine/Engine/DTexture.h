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
	DWrapMode GetWrapMode();
	//DTextureBuffer* GetTextureBuffer();

	static DTexture2D* Create(WCHAR*);
	static DTexture2D* Create(WCHAR*, DWrapMode);

private:
	DTextureRes* m_textureRes;
	DWrapMode m_wrapMode;
};

class DTextureCube : public DTexture
{

};

class DRenderTexture : public DTexture
{

};

