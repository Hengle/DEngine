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
private:
	DRenderTexture();
public:
	~DRenderTexture();
	virtual void Destroy();
	virtual void Apply(UINT);
	DWrapMode GetWrapMode();
	DRenderTextureViewRes* GetTextureRes();

	static DRenderTexture* Create(float, float);
	static DRenderTexture* Create(float, float, DWrapMode);


private:
	DRenderTextureViewRes* m_renderTextureRes;
	DWrapMode m_wrapMode;
};

