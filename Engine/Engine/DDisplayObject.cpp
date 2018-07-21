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

DMaterial * DDisplayObject::GetMaterial()
{
	return m_material;
}

DGeometry * DDisplayObject::GetGeometry()
{
	return m_geometry;
}

void DDisplayObject::GetBounds(DBounds * bounds)
{
	if (m_geometry && (m_geometry->IsBoundsRangeChanged()) || m_transform->IsAreaChanged())
	{
		UpdateBounds();
	}
	*bounds = m_bounds;
}

bool DDisplayObject::OnInit()
{
	m_gizmoMat = DRes::Load<DMaterial>(0, 3004);
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

bool DDisplayObject::OnCullObject(DCuller * culler)
{
	if (m_geometry != NULL && m_material != NULL && m_isVisible)
	{
		DGraphics::PushRenderQueue(this, m_material->GetRenderQueue());
		return true;
	}
	return false;
}

void DDisplayObject::OnRenderObject()
{
	m_gizmoMat->SetPass(0);
	DBounds bounds = m_bounds;
	DGraphics::GlPushMatrix();

	DGraphics::GlBegin();

	DGraphics::GlColor(DCOLOR_GREEN);


	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);


	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);


	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z + bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);

	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);
	DGraphics::GlVertex3(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f, bounds.center.z - bounds.size.z * 0.5f);

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();

	if (m_geometry != NULL && m_material != NULL && m_isVisible)
	{
		DShader* rpshader = DGraphics::GetGlobalRenderShader();
		DMatrix4x4 world;
		m_transform->GetLocalToWorld(world);
		if (rpshader != NULL)
		{
			DShader* current = m_material->GetShader();
			m_material->SetShader(rpshader);
			DGraphics::DrawGeometry(m_geometry, world, m_material);
			m_material->SetShader(current);
		}
		else
		{
			DGraphics::DrawGeometry(m_geometry, world, m_material);
		}
	}
}

void DDisplayObject::UpdateBounds()
{
	DVector3 rmin, rmax;
	m_geometry->GetBoundsRange(&rmin, &rmax);



	m_transform->ClearAreaChange();
}
