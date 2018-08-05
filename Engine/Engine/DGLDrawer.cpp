#include "DGLDrawer.h"
#include "DSystem.h"

DGLDrawer::DGLDrawer()
{
	m_currentMV = dmat_identity;
	m_currentP = dmat_identity;
	//m_currentPLen = 0;
	//m_prePLen = 0;
	m_geometryRes = 0;
	m_vertexCount = 0;
	m_indexCount = 0;
}


DGLDrawer::~DGLDrawer()
{
}

//void DGLDrawer::PostGL()
//{
//	/*if (m_currentPLen != m_prePLen)
//	{
//		GenerateNewProcesses();
//		m_prePLen = m_currentPLen;
//	}
//	m_currentPLen = 0;*/
//
//}

void DGLDrawer::GlBegin()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_vertices.clear();
	m_indecies.clear();
	/*if (m_currentPLen < m_prePLen)
	{
		m_currentProcess = m_processVector.at(m_currentPLen);
	}
	m_currentPLen += 1;*/
}

void DGLDrawer::GlEnd()
{
	if (m_vertexCount == 0 || m_indexCount == 0)
		return;
	if (m_geometryRes == NULL)
	{
		int vusage = (1UL << DVertexUsage_POSITION) | (1UL << DVertexUsage_COLOR);
		m_geometryRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateGeometryRes(vusage, true);
	}
	m_geometryRes->Refresh(&m_vertices[0], &m_indecies[0], m_vertexCount, m_indexCount);
	DMatrix4x4 world;
	DMatrix4x4::Identity(&world);

	DShader::SetGlobalMatrix(D_SC_MATRIX_M, world);
	DShader::SetGlobalMatrix(D_SC_MATRIX_V, m_currentMV);
	DShader::SetGlobalMatrix(D_SC_MATRIX_P, m_currentP);

	DGraphics::ApplyActiveMaterial();
	m_geometryRes->DrawPrimitive(DGeometryTopology_LineList);
	/*if (m_currentProcess != NULL)
	{
		m_currentProcess->PostProcess(m_currentMV, m_currentP);
	}
	m_currentProcess = NULL;*/
}

void DGLDrawer::GlVertex(DVector3 * vector)
{
	/*if (m_currentProcess == NULL)
		return;
	m_currentProcess->ProcessVector(vector->x, vector->y, vector->z);*/
	m_vertices.push_back(vector->x);
	m_vertices.push_back(vector->y);
	m_vertices.push_back(vector->z);
	m_vertices.push_back(m_currentColor.r);
	m_vertices.push_back(m_currentColor.g);
	m_vertices.push_back(m_currentColor.b);
	m_vertices.push_back(m_currentColor.a);
	m_indecies.push_back(m_vertexCount);
	m_vertexCount += 1;
	m_indexCount += 1;
}

void DGLDrawer::GlVertex3(float x, float y, float z)
{
	/*if (m_currentProcess == NULL)
		return;
	m_currentProcess->ProcessVector(x, y, z);*/
	m_vertices.push_back(x);
	m_vertices.push_back(y);
	m_vertices.push_back(z);
	m_vertices.push_back(m_currentColor.r);
	m_vertices.push_back(m_currentColor.g);
	m_vertices.push_back(m_currentColor.b);
	m_vertices.push_back(m_currentColor.a);
	m_indecies.push_back(m_vertexCount);
	m_vertexCount += 1;
	m_indexCount += 1;
}

void DGLDrawer::GlColor(DColor * color)
{
	m_currentColor = *color;
	/*if (m_currentProcess == NULL)
		return;
	m_currentProcess->ProcessColor(color);*/
	
}

void DGLDrawer::GlPushMatrix()
{
	m_MVMatrixStack.push(m_currentMV);
	m_PMatrixStack.push(m_currentP);
}

void DGLDrawer::GlPopMatrix()
{
	if (!m_MVMatrixStack.empty())
	{
		m_currentMV = m_MVMatrixStack.top();
		m_MVMatrixStack.pop();
	}
	if (!m_PMatrixStack.empty())
	{
		m_currentP = m_PMatrixStack.top();
		m_PMatrixStack.pop();
	}
}

void DGLDrawer::GlLoadIdentity()
{
	DMatrix4x4::Identity(&m_currentMV);
}

void DGLDrawer::GlLoadProjectionMatrix(DMatrix4x4& projection)
{
	m_currentP = projection;
}

void DGLDrawer::GlLoadOrtho()
{
	m_currentMV = DMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 1.0f);
	//float screenWidth, screenHeight;
	//DSystem::GetGraphicsMgr()->GetGLCore()->GetResolution(screenWidth, screenHeight);
	DMatrix4x4::Ortho(&m_currentP, 2.0f, 2.0f, -1.0f, 1.0f);
}

void DGLDrawer::GlMultiMatrix(DMatrix4x4& matrix)
{
	m_currentMV = matrix * m_currentMV;
}

//void DGLDrawer::GlSetMaterial(DMaterial * material)
//{
//	m_material = material;
//}

void DGLDrawer::Release()
{
	if (m_geometryRes != NULL)
	{
		m_geometryRes->Release();
		delete m_geometryRes;
		m_geometryRes = 0;
	}
	/*int i = 0;
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
	m_processVector.clear();*/
	
}

void DGLDrawer::GetModelView(DMatrix4x4 & out)
{
	out = m_currentMV;
}

void DGLDrawer::GetProjection(DMatrix4x4 & out)
{
	out = m_currentP;
}

//void DGLDrawer::GenerateNewProcesses()
//{
//	if (m_currentPLen > m_prePLen)
//	{
//		int i;
//		for (i = m_prePLen; i < m_currentPLen; i++)
//		{
//			m_processVector.push_back(new DGLDrawerProcess());
//		}
//	}
//	else if (m_currentPLen < m_prePLen)
//	{
//		int i;
//		for (i = m_currentPLen; i < m_prePLen; i++)
//		{
//			DGLDrawerProcess* process = m_processVector.back();
//			if (process != NULL)
//			{
//				process->Release();
//				delete process;
//			}
//			m_processVector.pop_back();
//		}
//	}
//}

//DGLDrawerProcess::DGLDrawerProcess()
//{
//	m_currentColor = dcol_white;
//	m_currentIndex = 0;
//	m_preIndex = 0;
//	m_vertices = 0;
//	m_indices = 0;
//	m_geometryRes = 0;
//	m_hasDrawCommand = false;
//	//m_geoDesc.indices = 0;
//	//m_geoDesc.vertices = 0;
//	//m_geoDesc.colors = 0;
//}
//
//void DGLDrawerProcess::Release()
//{
//	if (m_geometryRes != NULL)
//	{
//		m_geometryRes->Release();
//		delete m_geometryRes;
//		m_geometryRes = 0;
//	}
//	/*if (m_geoDesc.vertices != 0)
//	{
//		delete[] m_geoDesc.vertices;
//		m_geoDesc.vertices = 0;
//	}
//	if (m_geoDesc.indices != 0)
//	{
//		delete[] m_geoDesc.indices;
//		m_geoDesc.indices = 0;
//	}
//	if (m_geoDesc.colors != 0)
//	{
//		delete[] m_geoDesc.colors;
//		m_geoDesc.colors = 0;
//	}*/
//}
//
//void DGLDrawerProcess::ProcessVector(float x, float y, float z)
//{
//	if (m_currentIndex < m_preIndex)
//	{
//		/*m_vertices[m_currentIndex * 7] = x;
//		m_vertices[m_currentIndex * 7 + 1] = y;
//		m_vertices[m_currentIndex * 7 + 2] = z;
//		m_vertices[m_currentIndex * 7 + 3] = m_currentColor.r;
//		m_vertices[m_currentIndex * 7 + 4] = m_currentColor.g;
//		m_vertices[m_currentIndex * 7 + 5] = m_currentColor.b;
//		m_vertices[m_currentIndex * 7 + 6] = m_currentColor.a;
//		m_indices[m_currentIndex] = m_currentIndex;*/
//		/*m_geoDesc.vertices[m_currentIndex * 3] = x;
//		m_geoDesc.vertices[m_currentIndex * 3 + 1] = y;
//		m_geoDesc.vertices[m_currentIndex * 3 + 2] = z;
//
//		m_geoDesc.colors[m_currentIndex * 4] = m_currentColor.r;
//		m_geoDesc.colors[m_currentIndex * 4 + 1] = m_currentColor.g;
//		m_geoDesc.colors[m_currentIndex * 4 + 2] = m_currentColor.b;
//		m_geoDesc.colors[m_currentIndex * 4 + 3] = m_currentColor.a;
//		m_geoDesc.indices[m_currentIndex] = m_currentIndex;*/
//	}
//	m_currentIndex += 1;
//	m_hasDrawCommand = true;
//}
//
//void DGLDrawerProcess::ProcessColor(DColor * color)
//{
//	m_currentColor = *color;
//	m_hasDrawCommand = true;
//}
//
//void DGLDrawerProcess::PostProcess(DMatrix4x4& modelview, DMatrix4x4& projection)
//{
//	//if (m_geoDesc.vertices != 0 && m_geoDesc.colors != 0 && m_geoDesc.indices != 0 && m_hasDrawCommand)
//	//{
//	//	if (m_geometryRes == NULL)
//	//	{
//	//		int vusage = (1UL << DVertexUsage_POSITION) | (1UL << DVertexUsage_COLOR);
//	//		m_geometryRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateGeometryRes(vusage, true);
//	//	}
//	//	m_geoDesc.vertexCount = m_currentIndex;
//	//	m_geoDesc.indexCount = m_currentIndex;
//	//	//m_geometryRes->Refresh(m_vertices, m_indices, m_currentIndex * 3, m_currentIndex);
//	//	m_geometryRes->Refresh(&m_geoDesc);
//	//	ProcessDraw(modelview, projection);
//	//	m_hasDrawCommand = false;
//	//}
//	//if (m_currentIndex != m_preIndex)
//	//{
//	//	if (m_geoDesc.vertices != 0)
//	//	{
//	//		delete[] m_geoDesc.vertices;
//	//		m_geoDesc.vertices = 0;
//	//	}
//	//	m_geoDesc.vertices = new float[m_currentIndex * 3];
//	//	if (m_geoDesc.colors != 0)
//	//	{
//	//		delete[] m_geoDesc.colors;
//	//		m_geoDesc.colors = 0;
//	//	}
//	//	m_geoDesc.colors = new float[m_currentIndex * 4];
//	//	if (m_geoDesc.indices != 0)
//	//	{
//	//		delete[] m_geoDesc.indices;
//	//		m_geoDesc.indices = 0;
//	//	}
//	//	m_geoDesc.indices = new unsigned long[m_currentIndex];
//	//	m_preIndex = m_currentIndex;
//	//}
//	//m_currentIndex = 0;
//}
//
//void DGLDrawerProcess::ProcessDraw(DMatrix4x4& modelview, DMatrix4x4& projection)
//{
//
//	DMatrix4x4 world;
//	DMatrix4x4::Identity(&world);
//
//	DShader::SetGlobalMatrix(D_SC_MATRIX_M, world);
//	DShader::SetGlobalMatrix(D_SC_MATRIX_V, modelview);
//	DShader::SetGlobalMatrix(D_SC_MATRIX_P, projection);
//
//	DGraphics::ApplyActiveMaterial();
//	m_geometryRes->DrawPrimitive(DGeometryTopology_LineList);
//}
