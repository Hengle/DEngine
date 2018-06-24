#pragma once
#include "D3D11Core.h"
#include "DMath.h"
#include "DColor.h"

class DGLDrawer11 : public DGLDrawer
{
public:
	DGLDrawer11();
	~DGLDrawer11();

	virtual void glBegin();
	virtual void glEnd();
	virtual void glVector3(DVector3*);
	virtual void glColor(DColor*);

private:
	float* m_vertices;
	unsigned int* m_indices;

	DColor m_currentColor;

	unsigned int m_currentVCount, m_currentICount, m_preVCount, m_preICount;
};

