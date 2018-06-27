#include "DGLDrawer11.h"
#include "DSystem.h"

DGLDrawer11::DGLDrawer11()
{
	m_currentColor = dcol_white;
	m_currentIndex = 0;
	m_preIndex = 0;
	m_vertices = 0;
	m_indices = 0;
	m_meshRes = 0;
}


DGLDrawer11::~DGLDrawer11()
{
}

void DGLDrawer11::PrepareGL()
{
	if (m_vertices != 0 && m_indices != 0)
	{
		if (m_meshRes == NULL)
		{
			int vusage = (1UL << DVertexUsage_POSITION) | (1UL << DVertexUsage_POSITION);
			m_meshRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateMeshRes(vusage);
		}
		DMeshBufferDesc desc;
		m_meshRes->Refresh()
	}
	if (m_currentIndex != m_preIndex)
	{
		if (m_vertices != 0)
		{
			delete[] m_vertices;
			m_vertices = 0;
		}
		m_vertices = new float[m_currentIndex * 3];
		if (m_indices != 0)
		{
			delete[] m_indices;
			m_indices = 0;
		}
		m_indices = new unsigned long[m_currentIndex];
		m_preIndex = m_currentIndex;
		m_currentIndex = 0;
	}
}

void DGLDrawer11::GlBegin()
{
}

void DGLDrawer11::GlEnd()
{
}

void DGLDrawer11::GlVector3(DVector3 * vector)
{
	if (m_currentIndex < m_preIndex)
	{
		m_vertices[m_currentIndex * 3] = vector->x;
		m_vertices[m_currentIndex * 3 + 1] = vector->y;
		m_vertices[m_currentIndex * 3 + 2] = vector->z;
		m_indices[m_currentIndex] = m_currentIndex;
	}
	m_currentIndex += 1;
}

void DGLDrawer11::GlColor(DColor * color)
{
	m_currentColor = *color;
}

void DGLDrawer11::GlPushMatrix()
{
}

void DGLDrawer11::GlPopMatrix()
{
}

void DGLDrawer11::GlLoadIdentity()
{
}

void DGLDrawer11::Release()
{
}
