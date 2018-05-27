#include "DTransform.h"
#include <math.h>

DTransform::DTransform()
{
	m_position = DVector3(0,0,0);
	//m_euler = D3DXVECTOR3();
	m_scale = DVector3(1, 1, 1);
	m_up = DVector3(0, 1, 0);
	m_forward = DVector3(0, 0, 1);
	m_rotation = DQuaterion(0, 0, 0, 1);
	m_euler = DVector3(0, 0, 0);

	m_isMatrixChanged = false;
	m_isEulerChanged = false;

	D3DXMatrixIdentity(&m_localToWorld);
}

DTransform::~DTransform()
{
}

void DTransform::SetPosition(FLOAT x, FLOAT y, FLOAT z)
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

void DTransform::SetRotation(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
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

void DTransform::SetEuler(FLOAT pitch, FLOAT yaw, FLOAT roll)
{
	if (IS_FLOAT_EQUAL(m_euler.x, pitch) && IS_FLOAT_EQUAL(m_euler.y, yaw) && IS_FLOAT_EQUAL(m_euler.z, roll))
		return;
	m_euler.x = pitch;
	m_euler.y = yaw;
	m_euler.z = roll;
	D3DXQuaternionRotationYawPitchRoll(&m_rotation, m_euler.y*DEG_TO_RAD, m_euler.x*DEG_TO_RAD, m_euler.z*DEG_TO_RAD);
	m_isMatrixChanged = true;
}

void DTransform::SetEuler(DVector3 euler)
{
	if (IS_FLOAT_EQUAL(m_euler.x, euler.x) && IS_FLOAT_EQUAL(m_euler.y, euler.y) && IS_FLOAT_EQUAL(m_euler.z, euler.z))
		return;
	m_euler = euler;
	D3DXQuaternionRotationYawPitchRoll(&m_rotation, m_euler.x*DEG_TO_RAD, m_euler.y*DEG_TO_RAD, m_euler.z*DEG_TO_RAD);
	m_isMatrixChanged = true;
}

void DTransform::SetScale(FLOAT x, FLOAT y, FLOAT z)
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
	D3DXVec3Cross(&right, &m_up, &m_forward);
}

void DTransform::GetLocalToWorld(DMatrix4x4 & localToWorld)
{
	if (!m_isMatrixChanged) {
		localToWorld = m_localToWorld;
		return;
	}
	D3DXMatrixIdentity(&m_localToWorld);
	DMatrix4x4 t;

	RefreshMatrix();
	
	localToWorld = m_localToWorld;
}

bool DTransform::IsMatrixWillChange()
{
	return m_isMatrixChanged;
}

void DTransform::RefreshMatrix()
{
	DMatrix4x4 t;
	m_isMatrixChanged = false;

	D3DXMatrixScaling(&t, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_localToWorld, &m_localToWorld, &t);

	D3DXMatrixRotationQuaternion(&t, &m_rotation);
	m_up = dvec3_up;
	m_forward = dvec3_forward;
	D3DXVec3TransformCoord(&m_forward, &m_forward, &t);
	D3DXVec3TransformCoord(&m_up, &m_up, &t);
	D3DXMatrixMultiply(&m_localToWorld, &m_localToWorld, &t);

	D3DXMatrixTranslation(&t, m_position.x, m_position.y, m_position.z);
	D3DXMatrixMultiply(&m_localToWorld, &m_localToWorld, &t);
}

void DTransform::RefreshEuler()
{
	float x, y, z, w;
	x = m_rotation.x;
	y = m_rotation.y;
	z = m_rotation.z;
	w = m_rotation.w;

	long sp = -2.0f*(y*z - w*x);
	
	if (abs(sp) > 0.9999f)
	{
		m_euler.x = 1.570796f * sp;
		m_euler.y = atan2(-x*z + w*y, 0.5f - y*y - z*z);
		m_euler.z = 0.0f;
	}
	else
	{
		m_euler.x = asin(sp);
		m_euler.y = atan2(x*z + w*y, 0.5f - x*x - y*y);
		m_euler.z = atan2(x*y + w*z, 0.5f - x*x - z*z);
	}
	m_euler.x *= RAD_TO_DEG;
	m_euler.y *= RAD_TO_DEG;
	m_euler.z *= RAD_TO_DEG;
}

