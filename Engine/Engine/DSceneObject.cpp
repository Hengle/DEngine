#include "DSceneObject.h"
#include "DSystem.h"

DSceneObject::DSceneObject()
{
	m_isInitialized = false;
	m_transform = new DTransform(this);
}


DSceneObject::~DSceneObject()
{
}

void DSceneObject::Create()
{
	DSystem::GetSceneMgr()->GetCurrentScene()->AddTransform(m_transform);
	m_isInitialized = OnInit();
}

void DSceneObject::Destroy()
{
	if (m_transform != NULL)
	{
		m_transform->RemoveFromParent();
		m_transform->Release();
		delete m_transform;
		m_transform = NULL;
	}
	OnDestroy();
}

void DSceneObject::Update()
{
	if (m_isInitialized)
		OnUpdate();
}

void DSceneObject::FixedUpdate()
{
	if (m_isInitialized)
		OnFixedUpdate();
}

void DSceneObject::Render()
{
	if (m_isInitialized)
		OnRender();
}

void DSceneObject::Cull()
{
	if (m_isInitialized)
		OnCull();
}

DTransform * DSceneObject::GetTransform() const
{
	return m_transform;
}

bool DSceneObject::IsInitialized()
{
	return m_isInitialized;
}
