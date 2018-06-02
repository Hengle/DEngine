#include "DSceneObject.h"


DSceneObject::DSceneObject()
{
	m_Transform = new DTransform();
}


DSceneObject::~DSceneObject()
{
	if (m_Transform != NULL) {
		delete m_Transform;
		m_Transform = NULL;
	}
}

void DSceneObject::Init()
{
}

void DSceneObject::Destroy()
{

}

void DSceneObject::Update()
{
}

void DSceneObject::FixedUpdate()
{
}

DTransform * DSceneObject::GetTransform() const
{
	return m_Transform;
}