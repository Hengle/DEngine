#pragma once
#if _DGAPI_OPENGL

#include "DOpenGLCore.h"

class DColorBufferOpGL : public IRenderBuffer
{
public:
	DColorBufferOpGL();
	~DColorBufferOpGL();
	virtual void Release();
	//static DColorBufferOpGL* Create(ID3D11Device*, ID3D11Resource*, const D3D11_RENDER_TARGET_VIEW_DESC*);

};

class DDepthBufferOpGL : public IRenderBuffer
{
public:
	DDepthBufferOpGL();
	~DDepthBufferOpGL();
	virtual void Release();
	//static DDepthBufferOpGL* Create(ID3D11Device*, ID3D11Resource*, const D3D11_DEPTH_STENCIL_VIEW_DESC*);

};

#endif