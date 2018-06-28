#include "DGLDrawer.h"
#include "DSystem.h"

DGLDrawer::DGLDrawer()
{
	m_currentMatrix = dmat_identity;
	m_currentPLen = 0;
	m_prePLen = 0;
}


DGLDrawer::~DGLDrawer()
{
}

void DGLDrawer::PostGL()
{
	if (m_currentPLen != m_prePLen)
	{
		GenerateNewProcesses();
		m_prePLen = m_currentPLen;
	}
	m_currentPLen = 0;

}

void DGLDrawer::GlBegin()
{
	if (m_currentPLen < m_prePLen)
	{
		m_currentProcess = m_processVector.at(m_currentPLen);
	}
	m_currentPLen += 1;
}

void DGLDrawer::GlEnd()
{
	if (m_currentProcess != NULL)
	{
		m_currentProcess->PostProcess(m_material, m_currentMatrix);
	}
	m_currentProcess = NULL;
}

void DGLDrawer::GlVector3(DVector3 * vector)
{
	if (m_currentProcess == NULL)
		return;
	m_currentProcess->ProcessVector(vector->x, vector->y, vector->z);
}

void DGLDrawer::GlVector(float x, float y, float z)
{
	if (m_currentProcess == NULL)
		return;
	m_currentProcess->ProcessVector(x, y, z);
}

void DGLDrawer::GlColor(DColor * color)
{
	if (m_currentProcess == NULL)
		return;
	m_currentProcess->ProcessColor(color);
	
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

void DGLDrawer::GlSetMaterial(DMaterial * material)
{
	m_material = material;
}

void DGLDrawer::Release()
{
	int i = 0;
	int len = m_processVector.size();
	if (len > 0)
	{
		for (i = 0; i < len; i++)
		{
			DGLDrawerProcess* process = m_processVector.at(i);
			process->Release();
			delete process;
		}
	}
	m_processVector.clear();
	m_material = NULL;
}

void DGLDrawer::GenerateNewProcesses()
{
	if (m_currentPLen > m_prePLen)
	{
		int i;
		for (i = m_prePLen; i < m_currentPLen; i++)
		{
			m_processVector.push_back(new DGLDrawerProcess());
		}
	}
	else if (m_currentPLen < m_prePLen)
	{
		int i;
		for (i = m_currentPLen; i < m_prePLen; i++)
		{
			DGLDrawerProcess* process = m_processVector.back();
			if (process != NULL)
			{
				process->Release();
				delete process;
			}
			m_processVector.pop_back();
		}
	}
}

DGLDrawerProcess::DGLDrawerProcess()
{
	m_currentColor = dcol_white;
	m_currentIndex = 0;
	m_preIndex = 0;
	m_vertices = 0;
	m_indices = 0;
	m_meshRes = 0;
	m_hasDrawCommand = false;
}

void DGLDrawerProcess::Release()
{
	if (m_meshRes != NULL)
	{
		m_meshRes->Release();
		delete m_meshRes;
		m_meshRes = 0;
	}
	if (m_vertices != 0)
	{
		delete[] m_vertices;
		m_vertices = 0;
	}
	if (m_indices != 0)
	{
		delete[] m_indices;
		m_indices = 0;
	}
	
}

void DGLDrawerProcess::ProcessVector(float x, float y, float z)
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
	m_hasDrawCommand = true;
}

void DGLDrawerProcess::ProcessColor(DColor * color)
{
	m_currentColor = *color;
	m_hasDrawCommand = true;
}

void DGLDrawerProcess::PostProcess(DMaterial* material, DMatrix4x4& matrix)
{
	if (m_vertices != 0 && m_indices != 0 && m_hasDrawCommand)
	{
		if (m_meshRes == NULL)
		{
			int vusage = (1UL << DVertexUsage_POSITION) | (1UL << DVertexUsage_COLOR);
			m_meshRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateMeshRes(vusage, true);
		}
		m_meshRes->Refresh(m_vertices, m_indices, m_currentIndex * 3, m_currentIndex);
		ProcessDraw(material, matrix);
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

void DGLDrawerProcess::ProcessDraw(DMaterial * material, DMatrix4x4& matrix)
{

	DCamera* camera;
	DCamera::GetCurrentCamera(&camera);

	DMatrix4x4 view, proj;
	camera->GetViewMatrix(view);
	camera->GetProjection(proj);

	material->SetMatrix("worldMatrix", matrix);
	material->SetMatrix("viewMatrix", view);
	material->SetMatrix("projectionMatrix", proj);

	material->Apply();

	m_meshRes->Draw(DMeshTopology_LineList);
}
