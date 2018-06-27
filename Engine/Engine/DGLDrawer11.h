#pragma once
#include "D3D11Core.h"
#include "DMath.h"
#include "DColor.h"
#include "DMesh.h"

class DGLDrawer11 : public DGLDrawer
{
public:
	DGLDrawer11();
	~DGLDrawer11();

	virtual void PrepareGL();
	virtual void GlBegin();
	virtual void GlEnd();
	virtual void GlVector3(DVector3*);
	virtual void GlColor(DColor*);
	virtual void GlPushMatrix();
	virtual void GlPopMatrix();
	virtual void GlLoadIdentity();
	virtual void Release();

private:
	float* m_vertices;
	unsigned long* m_indices;

	DColor m_currentColor;
	DMatrix4x4 m_currentMatrix;
	DMeshRes* m_meshRes;

	unsigned long m_currentIndex, m_preIndex;
};

