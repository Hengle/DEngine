#include "DDisplayObject.h"
#include "DSystem.h"
#include "DCuller.h"

DDisplayObject::DDisplayObject(DGeometry* geometry, DMaterial* material) : DSceneObject()
{
	m_material = material;
	m_geometry = geometry;
	m_isVisible = true;
	testUseGrab = false;
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

bool DDisplayObject::OnCullObject()
{
	if (m_geometry != NULL && m_material != NULL && m_isVisible)
	{
		DBounds bounds;
		GetBounds(&bounds);
		if (DCuller::Culling(bounds)) {
			DGraphics::PushRenderQueue(this, m_material->GetRenderQueue());
			return true;
		}
	}
	return false;
}

void DDisplayObject::OnRenderObject()
{
	if (testUseGrab)
		DGraphics::GrabScreenTexture();
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
	DVector3 center, size;
	m_geometry->GetBoundsRange(&center, &size);

	DMatrix4x4 ltw;
	m_transform->GetLocalToWorld(ltw);

	int i, j;
	for (i = 0; i < 3; i++)
	{
		m_bounds.center[i] = ltw[12+i];
		m_bounds.halfSize[i] = 0.0f;
		for (j = 0; j < 3; j++)
		{
			m_bounds.center[i] += ltw[j * 4 + i] * center[j];
			m_bounds.halfSize[i] += fabsf(ltw[j * 4 + i])*size[j]*0.5f;
		}
	}

	m_transform->ClearAreaChange();
}
