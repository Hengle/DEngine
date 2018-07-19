#include "DDisplayObject.h"
#include "DSystem.h"

DDisplayObject::DDisplayObject(DGeometry* geometry, DMaterial* material) : DSceneObject()
{
	m_material = material;
	m_geometry = geometry;
	m_isVisible = true;
}


DDisplayObject::~DDisplayObject()
{
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
	m_material = NULL;
	m_geometry = NULL;
}

void DDisplayObject::OnUpdate()
{
}

void DDisplayObject::OnFixedUpdate()
{
}

//void DDisplayObject::OnRenderObject()
//{
//	if (m_geometry != NULL && m_material != NULL && m_isVisible)
//	{
//		//DMatrix4x4 world;
//		//m_transform->GetLocalToWorld(world);
//		//DGraphics::PushRenderCommand(m_mesh, world, m_material);
//		DShader* rpshader = DGraphics::GetGlobalRenderShader();
//		DMatrix4x4 world;
//		//DCamera* cur;
//		//DCamera::GetCurrentCamera(&cur);
//		m_transform->GetLocalToWorld(world);
//		if (rpshader != NULL)
//		{
//			DShader* current = m_material->GetShader();
//			m_material->SetShader(rpshader);
//			DGraphics::DrawGeometry(m_geometry, world, m_material);
//			m_material->SetShader(current);
//		}
//		else
//		{
//			DGraphics::DrawGeometry(m_geometry, world, m_material);
//		}
//	}
//}

bool DDisplayObject::OnCullObject(DCuller * culler, DRender * render)
{
	if (m_geometry != NULL && m_material != NULL && m_isVisible)
	{
		render->PushDisplayObject(this, m_material->GetRenderQueue());
		return true;
	}
	return false;
}
