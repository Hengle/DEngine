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
		m_Transform->GetLocalToWorld(world);
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

void DDisplayObject::Init()
{
	DSceneObject::Init();
	if (m_material != NULL)
	{
		//m_shader->Init(DSystem::GetGraphicsMgr()->GetDevice());
	}
}

void DDisplayObject::Destroy()
{
	DSceneObject::Destroy();
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

void DDisplayObject::SetVisible(bool visible)
{
	m_isVisible = visible;
}

bool DDisplayObject::GetVisible()
{
	return m_isVisible;
}
