#include "DGLDrawer.h"
#include "DSystem.h"

DGLDrawer::DGLDrawer()
{
	m_currentColor = dcol_white;
	m_currentMatrix = dmat_identity;
	m_currentIndex = 0;
	m_preIndex = 0;
	m_vertices = 0;
	m_indices = 0;
	m_meshRes = 0;
	m_hasDrawCommand = false;
}


DGLDrawer::~DGLDrawer()
{
}

void DGLDrawer::PostGL()
{
	if (m_vertices != 0 && m_indices != 0 && m_hasDrawCommand)
	{
		if (m_meshRes == NULL)
		{
			int vusage = (1UL << DVertexUsage_POSITION) | (1UL << DVertexUsage_COLOR);
			m_meshRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateMeshRes(vusage);
		}
		m_meshRes->Refresh(m_vertices, m_indices, m_currentIndex * 3, m_currentIndex);
		Draw();
		m_hasDrawCommand = false;
	}
	if (m_currentIndex != m_preIndex)
	{
		if (m_vertices != 0)
		{
			delete[] m_vertices;
			m_vertices = 0;
		}
		m_vertices = new float[m_currentIndex * 7];
		if (m_indices != 0)
		{
			delete[] m_indices;
			m_indices = 0;
		}
		m_indices = new unsigned long[m_currentIndex];
		m_preIndex = m_currentIndex;
	}
	m_currentIndex = 0;
}

void DGLDrawer::GlBegin()
{
	m_hasDrawCommand = true;
}

void DGLDrawer::GlEnd()
{
}

void DGLDrawer::GlVector3(DVector3 * vector)
{
	if (m_currentIndex < m_preIndex)
	{
		m_vertices[m_currentIndex * 7] = vector->x;
		m_vertices[m_currentIndex * 7 + 1] = vector->y;
		m_vertices[m_currentIndex * 7 + 2] = vector->z;
		m_vertices[m_currentIndex * 7 + 3] = m_currentColor.r;
		m_vertices[m_currentIndex * 7 + 4] = m_currentColor.g;
		m_vertices[m_currentIndex * 7 + 5] = m_currentColor.b;
		m_vertices[m_currentIndex * 7 + 6] = m_currentColor.a;
		m_indices[m_currentIndex] = m_currentIndex;
	}
	m_currentIndex += 1;
}

void DGLDrawer::GlVector(float x, float y, float z)
{
	if (m_currentIndex < m_preIndex)
	{
		m_vertices[m_currentIndex * 7] = x;
		m_vertices[m_currentIndex * 7 + 1] = y;
		m_vertices[m_currentIndex * 7 + 2] = z;
		m_vertices[m_currentIndex * 7 + 3] = m_currentColor.r;
		m_vertices[m_currentIndex * 7 + 4] = m_currentColor.g;
		m_vertices[m_currentIndex * 7 + 5] = m_currentColor.b;
		m_vertices[m_currentIndex * 7 + 6] = m_currentColor.a;
		m_indices[m_currentIndex] = m_currentIndex;
	}
	m_currentIndex += 1;
}

void DGLDrawer::GlColor(DColor * color)
{
	m_currentColor = *color;
}

void DGLDrawer::GlPushMatrix()
{
	m_matrixStack.push(m_currentMatrix);
}

void DGLDrawer::GlPopMatrix()
{
	if (!m_matrixStack.empty())
	{
		m_currentMatrix = m_matrixStack.top();
		m_matrixStack.pop();
	}
}

void DGLDrawer::GlLoadIdentity()
{
	DMatrix4x4::Identity(&m_currentMatrix);
}

void DGLDrawer::Release()
{
	if (m_meshRes != NULL)
	{
		m_meshRes->Release();
		delete m_meshRes;
		m_meshRes = 0;
	}
	if (m_material != NULL)
	{
		m_material->Destroy();
		delete m_material;
		m_material = 0;
	}
	if (m_shader != NULL)
	{
		m_shader->Destroy();
		delete m_shader;
		m_shader = 0;
	}
}

void DGLDrawer::Draw()
{
	if (m_shader == NULL && m_material == NULL)
	{
		m_shader = DShader::Create(L"../Res/color.vs", L"../Res/color.ps");
		m_material = new DMaterial(m_shader);
	}

	DCamera* camera;
	DCamera::GetCurrentCamera(&camera);

	DMatrix4x4 view, proj;
	camera->GetViewMatrix(view);
	camera->GetProjection(proj);

	m_material->SetMatrix("worldMatrix", m_currentMatrix);
	m_material->SetMatrix("viewMatrix", view);
	m_material->SetMatrix("projectionMatrix", proj);

	m_material->Apply();

	m_meshRes->Draw(DMeshTopology_LineList);
}
