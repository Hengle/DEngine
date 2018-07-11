#include "DDisplayObject.h"
#include "DSystem.h"

DDisplayObject::DDisplayObject(DMesh* mesh, DMaterial* material)
{
	m_material = material;
	m_mesh = mesh;
	m_isVisible = true;
}


DDisplayObject::~DDisplayObject()
{
}

void DDisplayObject::Render(DShader* replaceShader)
{
	if (m_mesh != NULL && m_material != NULL && m_isVisible)
	{
		DMatrix4x4 world;
		//DCamera* cur;
		//DCamera::GetCurrentCamera(&cur);
		m_transform->GetLocalToWorld(world);
		if (replaceShader != NULL)
		{
			DShader* current = m_material->GetShader();
			m_material->SetShader(replaceShader);
			DGraphics::DrawMesh(m_mesh, world, m_material);
			m_material->SetShader(current);
		}
		else
		{
			DGraphics::DrawMesh(m_mesh, world, m_material);
		}
	}
}

void DDisplayObject::SetVisible(bool visible)
{
	m_isVisible = visible;
}

bool DDisplayObject::GetVisible()
{
	return m_isVisible;
}

bool DDisplayObject::OnInit()
{
	return true;
}

void DDisplayObject::OnDestroy()
{
	if (m_material != NULL) {
		m_material->Destroy();
		delete m_material;
		m_material = NULL;
	}
	if (m_mesh != NULL)
	{
		m_mesh->Destroy();
		delete m_mesh;
		m_mesh = NULL;
	}
}

void DDisplayObject::OnUpdate()
{
}

void DDisplayObject::OnFixedUpdate()
{
}

void DDisplayObject::OnRender()
{
}

void DDisplayObject::OnCull()
{
	if (m_mesh != NULL && m_material != NULL && m_isVisible)
	{
		DMatrix4x4 world;
		m_transform->GetLocalToWorld(world);
		DGraphics::PushRenderCommand(m_mesh, world, m_material);
	}
}
