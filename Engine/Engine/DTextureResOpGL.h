#pragma once
#ifdef _DGAPI_OPENGL

#include "DOpenGLCore.h"

class DTextureResOpGL : public ITextureRes
{
public:
	DTextureResOpGL(WCHAR* filename, DWrapMode);
	DTextureResOpGL(DTextureResOpGL*, DTextureResOpGL*, DTextureResOpGL*, DTextureResOpGL*, DTextureResOpGL*, DTextureResOpGL*);
	~DTextureResOpGL();
	virtual void Apply(UINT, int);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();

private:
	unsigned char* LoadBMP(WCHAR* path, unsigned int&, unsigned int&);
	unsigned char* LoadTGA(WCHAR* path, unsigned int&, unsigned int&);
	GLint GetWrapMode(DWrapMode);

private:
	GLuint m_textureId;
	DWrapMode m_wrapMode;
	unsigned char* m_data;
	unsigned int m_width, m_height;
	GLenum m_textureType;
	bool m_isSuccess;
};

class DRenderTextureViewResOpGL : public IRenderTextureViewRes
{
public:
	DRenderTextureViewResOpGL(float, float, DWrapMode wrapMode);
	~DRenderTextureViewResOpGL();
	virtual void Apply(UINT, int);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();
	virtual IRenderBuffer* GetColorBuffer();
	virtual IRenderBuffer* GetDepthBuffer();
	void BindFBO();
	void UnBindFBO();

private:
	GLint GetWrapMode(DWrapMode);

private:
	GLuint m_frameBufferId;
	GLuint m_renderTextureId;
	bool m_isSuccess;
	DWrapMode m_wrapMode;
};

#endif