#include "DDisplayObject.h"
#include "DSystem.h"

DDisplayObject::DDisplayObject(DMesh* mesh, DShader* shader)
{
	m_shader = shader;
	m_mesh = mesh;
}


DDisplayObject::~DDisplayObject()
{
}

void DDisplayObject::Render()
{
	if (m_mesh != NULL && m_shader != NULL)
	{
		DMatrix4x4 world, view, projection;
		DGraphics::DrawMesh(m_mesh);
		DCamera* cur;
		DCamera::GetCurrentCamera(&cur);
		m_Transform->GetLocalToWorld(world);
		cur->GetViewMatrix(view);
		cur->GetProjection(projection);
		m_shader->Render(DSystem::GetGraphicsMgr()->GetDeviceContext(), m_mesh->GetIndexCount(), world, view, projection);
	}
}

void DDisplayObject::Init()
{
	DSceneObject::Init();
	if (m_shader != NULL)
	{
		m_shader->Init(DSystem::GetGraphicsMgr()->GetDevice());
	}
}

void DDisplayObject::Destroy()
{
	DSceneObject::Destroy();
	if (m_shader != NULL) {
		m_shader->Destroy();
		delete m_shader;
		m_shader = NULL;
	}
	if (m_mesh != NULL)
	{
		m_mesh->Destroy();
		delete m_mesh;
		m_mesh = NULL;
	}
}
