﻿#pragma once
#ifdef _DGAPI_OPENGL

#include "DOpenGLCore.h"

class DTextureResOpGL : public ITextureRes
{
public:
	DTextureResOpGL(WCHAR* filename);
	~DTextureResOpGL();
	virtual void Apply(UINT);
	virtual void ApplyWrapMode(UINT, DWrapMode);
	virtual void Release();

private:
	GLuint LoadBMP(WCHAR* path);
	GLuint LoadTGA(WCHAR* path, GLuint&);

private:
	GLuint m_textureId;
};

#endif