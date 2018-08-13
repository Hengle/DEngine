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
	virtual void Apply(UINT, int = 0);
};

/*2d纹理*/
class DTexture2D : public DTexture 
{
private:
	DTexture2D();
public:
	~DTexture2D();
	virtual void Destroy();
	virtual void Apply(UINT, int = 0);
	DWrapMode GetWrapMode();
	void SetWrapMode(DWrapMode);
	ITextureRes* GetTextureRes();
	static DTexture2D* Create(WCHAR*);
	static DTexture2D* Create(WCHAR*, DWrapMode);

private:
	ITextureRes* m_textureRes;
	DWrapMode m_wrapMode;
};

class DTextureCube : public DTexture
{
private:
	DTextureCube();
public:
	~DTextureCube();
	virtual void Destroy();
	virtual void Apply(UINT, int = 0);
	static DTextureCube* Create(DTexture2D * right, DTexture2D * left, DTexture2D * top, DTexture2D * bottom, DTexture2D * front, DTexture2D * back);

private:
	ITextureRes* m_textureRes;
};

class DRenderTexture : public DTexture
{
private:
	DRenderTexture(float, float);
public:
	~DRenderTexture();
	virtual void Destroy();
	virtual void Apply(UINT, int = 0);
	DWrapMode GetWrapMode();
	void SetWrapMode(DWrapMode);
	IRenderTextureViewRes* GetTextureRes();
	float GetWidth() const;
	float GetHeight() const;

	static DRenderTexture* Create(float, float);
	static DRenderTexture* Create(float, float, DWrapMode);
	static DRenderTexture* BackBuffer();


private:
	IRenderTextureViewRes* m_renderTextureRes;
	DWrapMode m_wrapMode;
	float m_width;
	float m_height;
};

