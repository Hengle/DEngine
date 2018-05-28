#include "DTransform.h"

DTransform::DTransform()
{
	m_position = DVector3(0, 0, 0);
	//m_euler = D3DXVECTOR3();
	m_scale = DVector3(1, 1, 1);
	m_up = DVector3(0, 1, 0);
	m_forward = DVector3(0, 0, 1);
	m_rotation = DQuaterion(0, 0, 0, 1);
	m_euler = DVector3(0, 0, 0);

	m_isMatrixChanged = false;
	m_isEulerChanged = false;

	m_localToWorld = dmat_identity;
}

DTransform::~DTransform()
{
}

void DTransform::SetPosition(float x, float y, float z)
{
	if (IS_FLOAT_EQUAL(m_position.x, x) && IS_FLOAT_EQUAL(m_position.y, y) && IS_FLOAT_EQUAL(m_position.z, z))
		return;
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_isMatrixChanged = true;
}

void DTransform::SetPosition(DVector3 position)
{
	if (IS_FLOAT_EQUAL(m_position.x, position.x) && IS_FLOAT_EQUAL(m_position.y, position.y) && IS_FLOAT_EQUAL(m_position.z, position.z))
		return;
	m_position = position;
	m_isMatrixChanged = true;
}

void DTransform::SetRotation(float x, float y, float z, float w)
{
	if (IS_FLOAT_EQUAL(m_rotation.x, x) && IS_FLOAT_EQUAL(m_rotation.y, y) && IS_FLOAT_EQUAL(m_rotation.z, z) && IS_FLOAT_EQUAL(m_rotation.w, w))
		return;
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
	m_rotation.w = w;
	m_isMatrixChanged = true;
	m_isEulerChanged = true;
}

void DTransform::SetRotation(DQuaterion rotation)
{
	if (IS_FLOAT_EQUAL(m_rotation.x, rotation.x) && IS_FLOAT_EQUAL(m_rotation.y, rotation.y) && IS_FLOAT_EQUAL(m_rotation.z, rotation.z) && IS_FLOAT_EQUAL(m_rotation.w, rotation.w))
		return;
	m_rotation = rotation;
	m_isMatrixChanged = true;
	m_isEulerChanged = true;
}

void DTransform::SetEuler(float pitch, float yaw, float roll)
{
	if (IS_FLOAT_EQUAL(m_euler.x, pitch) && IS_FLOAT_EQUAL(m_euler.y, yaw) && IS_FLOAT_EQUAL(m_euler.z, roll))
		return;
	m_euler.x = pitch;
	m_euler.y = yaw;
	m_euler.z = roll;
	DQuaterion::Euler(&m_rotation, m_euler);
	m_isMatrixChanged = true;
}

void DTransform::SetEuler(DVector3 euler)
{
	if (IS_FLOAT_EQUAL(m_euler.x, euler.x) && IS_FLOAT_EQUAL(m_euler.y, euler.y) && IS_FLOAT_EQUAL(m_euler.z, euler.z))
		return;
	m_euler = euler;
	DQuaterion::Euler(&m_rotation, m_euler);
	m_isMatrixChanged = true;
}

void DTransform::SetScale(float x, float y, float z)
{
	if (IS_FLOAT_EQUAL(m_scale.x, x) && IS_FLOAT_EQUAL(m_scale.y, y) && IS_FLOAT_EQUAL(m_scale.z, z))
		return;
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
	m_isMatrixChanged = true;
}

void DTransform::SetScale(DVector3 scale)
{
	if (IS_FLOAT_EQUAL(m_scale.x, scale.x) && IS_FLOAT_EQUAL(m_scale.y, scale.y) && IS_FLOAT_EQUAL(m_scale.z, scale.z))
		return;
	m_scale = scale;
	m_isMatrixChanged = true;
}

void DTransform::GetPosition(DVector3 &position)
{
	position = m_position;
}

void DTransform::GetRotation(DQuaterion & rotation)
{
	rotation = m_rotation;
}

void DTransform::GetEuler(DVector3 & euler)
{
	if (m_isEulerChanged) {
		RefreshEuler();
		m_isEulerChanged = false;
	}
	euler = m_euler;
}

void DTransform::GetScale(DVector3 & scale)
{
	scale = m_scale;
}

void DTransform::GetUp(DVector3 & up)
{
	if (m_isMatrixChanged) {
		RefreshMatrix();
	}
	up = m_up;
}

void DTransform::GetForward(DVector3 & forward)
{
	if (m_isMatrixChanged) {
		RefreshMatrix();
	}
	forward = m_forward;
}

void DTransform::GetRight(DVector3 & right)
{
	if (m_isMatrixChanged) {
		RefreshMatrix();
	}
	DVector3::Cross(right, m_up, m_forward);
}

void DTransform::GetLocalToWorld(DMatrix4x4 & localToWorld)
{
	if (!m_isMatrixChanged) {
		localToWorld = m_localToWorld;
		return;
	}

	RefreshMatrix();

	localToWorld = m_localToWorld;
}

bool DTransform::IsMatrixWillChange()
{
	return m_isMatrixChanged;
}

void DTransform::RefreshMatrix()
{
	m_isMatrixChanged = false;

	DMatrix4x4::TRS(&m_localToWorld, &m_forward, &m_up, m_position, m_rotation, m_scale);
}

void DTransform::RefreshEuler()
{
	m_rotation.EulerAngle(m_euler);
}

