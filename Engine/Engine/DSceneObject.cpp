#include "DSceneObject.h"
#include "DSystem.h"

DSceneObject::DSceneObject()
{
	m_isInitialized = false;
	m_isDestroyed = false;
	m_transform = new DTransform(this);
	m_layer = DLAYER_DEFAULT;
}


DSceneObject::~DSceneObject()
{
}

void DSceneObject::Create()
{
	DSystem::GetSceneMgr()->GetCurrentScene()->AddSceneObject(this);
	m_isInitialized = OnInit();
}

void DSceneObject::Destroy()
{
	if (m_isDestroyed)
		return;
	if (m_transform != NULL)
	{
		DTransform* child = m_transform->GetFirstChild();
		while (child != NULL)
		{
			DSceneObject* childobj = child->GetSceneObject();
			child = child->GetNextNegibhor();
			if (childobj != NULL)
				childobj->Destroy();
		}

		m_transform->RemoveFromParent();
		m_transform->Release();
		delete m_transform;
		m_transform = NULL;
	}
	OnDestroy();
	m_isDestroyed = true;
}

void DSceneObject::Update()
{
	if (m_isInitialized && !m_isDestroyed)
	{
		OnUpdate();
	}
}

void DSceneObject::FixedUpdate()
{
	if (m_isInitialized && !m_isDestroyed)
		OnFixedUpdate();
}

bool DSceneObject::CullObject()
{
	if (m_isInitialized && !m_isDestroyed)
		return OnCullObject();
	return false;
}

DTransform * DSceneObject::GetTransform() const
{
	return m_transform;
}

bool DSceneObject::IsInitialized()
{
	return m_isInitialized;
}

bool DSceneObject::IsDestroyed()
{
	return m_isDestroyed;
}

void DSceneObject::SetLayer(DLAYER layer)
{
	m_layer = layer;
}

DLAYER DSceneObject::GetLayer()
{
	return m_layer;
}
