#include "DGLDrawer11.h"


DGLDrawer11::DGLDrawer11()
{
	m_currentColor = dcol_white;
	m_currentICount = 0;
	m_currentVCount = 0;
	m_preICount = 0;
	m_preVCount = 0;
}


DGLDrawer11::~DGLDrawer11()
{
}

void DGLDrawer11::glBegin()
{
}

void DGLDrawer11::glEnd()
{
}

void DGLDrawer11::glVector3(DVector3 * vector)
{
	if (m_currentVCount < m_preVCount && m_currentICount < m_preICount)
	{
		
	}
}

void DGLDrawer11::glColor(DColor * color)
{
	m_currentColor = *color;
}
