#pragma once
#ifdef _DGAPI_OPENGL

#include "DOpenGLCore.h"

class DTextureResOpGL : public ITextureRes
{
public:
	DTextureResOpGL(WCHAR* filename, DWrapMode);
	~DTextureResOpGL();
	virtual void Apply(UINT, int);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();

private:
	GLuint LoadBMP(WCHAR* path);
	GLuint LoadTGA(WCHAR* path, GLuint&);
	GLint GetWrapMode(DWrapMode);

private:
	GLuint m_textureId;
	DWrapMode m_wrapMode;
};

#endif