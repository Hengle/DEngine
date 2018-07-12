#pragma once
#include <windows.h>
#include "DGLCore.h"
#include "DResObject.h"

/*纹理*/
class DTexture : public DResObject
{
public:
	DTexture();
	~DTexture();
	virtual void Apply(UINT);
};

/*2d纹理*/
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
	ITextureRes* m_textureRes;
	DWrapMode m_wrapMode;
};

class DTextureCube : public DTexture
{

};

class DRenderTexture : public DTexture
{
private:
	DRenderTexture(float, float);
public:
	~DRenderTexture();
	virtual void Destroy();
	virtual void Apply(UINT);
	DWrapMode GetWrapMode();
	IRenderTextureViewRes* GetTextureRes();
	float GetWidth() const;
	float GetHeight() const;

	static DRenderTexture* Create(float, float);
	static DRenderTexture* Create(float, float, DWrapMode);


private:
	IRenderTextureViewRes* m_renderTextureRes;
	DWrapMode m_wrapMode;
	float m_width;
	float m_height;
};

