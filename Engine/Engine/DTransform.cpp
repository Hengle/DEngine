#include "DTransform.h"

DTransform::DTransform()
{
	m_position = DVector3(0, 0, 0);
	//m_euler = D3DXVECTOR3();
	m_scale = DVector3(1, 1, 1);
	m_right = DVector3(1, 0, 0);
	m_up = DVector3(0, 1, 0);
	m_forward = DVector3(0, 0, 1);
	m_rotation = DQuaterion(0, 0, 0, 1);
	m_euler = DVector3(0, 0, 0);

	m_isL2WMatrixChanged = false;
	m_isW2LMatrixChanged = false;
	m_isEulerChanged = false;

	m_childCount = 0;

	m_localToWorld = dmat_identity;

	m_sceneObj = NULL;

	m_parent = NULL;
	m_preNeighbor = NULL;
	m_nextNeighbor = NULL;

	m_firstChild = NULL;

	m_isRoot = false;
}

DTransform::DTransform(bool root) : DTransform()
{
	m_isRoot = root;
}

DTransform::DTransform(DSceneObject * data) : DTransform()
{
	m_sceneObj = data;
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

	/*改变transform的坐标将引发矩阵的修改，该修改会延迟到下一帧或下一帧之后*/
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

void DTransform::SetPosition(DVector3 position)
{
	if (IS_FLOAT_EQUAL(m_position.x, position.x) && IS_FLOAT_EQUAL(m_position.y, position.y) && IS_FLOAT_EQUAL(m_position.z, position.z))
		return;
	m_position = position;

	/*改变transform的坐标将引发矩阵的修改，该修改会延迟到下一帧或下一帧之后*/
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

void DTransform::SetRotation(float x, float y, float z, float w)
{
	if (IS_FLOAT_EQUAL(m_rotation.x, x) && IS_FLOAT_EQUAL(m_rotation.y, y) && IS_FLOAT_EQUAL(m_rotation.z, z) && IS_FLOAT_EQUAL(m_rotation.w, w))
		return;
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
	m_rotation.w = w;

	/*改变transform的角度将引发矩阵的修改，该修改会延迟到下一帧或下一帧之后*/
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
	/*引起欧拉角的修改，该修改在下次访问欧拉角时生效*/
	m_isEulerChanged = true;
}

void DTransform::SetRotation(DQuaterion rotation)
{
	if (IS_FLOAT_EQUAL(m_rotation.x, rotation.x) && IS_FLOAT_EQUAL(m_rotation.y, rotation.y) && IS_FLOAT_EQUAL(m_rotation.z, rotation.z) && IS_FLOAT_EQUAL(m_rotation.w, rotation.w))
		return;
	m_rotation = rotation;

	/*改变transform的角度将引发矩阵的修改，该修改会延迟到下一帧或下一帧之后*/
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
	/*引起欧拉角的修改，该修改在下次访问欧拉角时生效*/
	m_isEulerChanged = true;
}

void DTransform::SetEuler(float pitch, float yaw, float roll)
{
	if (IS_FLOAT_EQUAL(m_euler.x, pitch) && IS_FLOAT_EQUAL(m_euler.y, yaw) && IS_FLOAT_EQUAL(m_euler.z, roll))
		return;
	m_euler.x = pitch;
	m_euler.y = yaw;
	m_euler.z = roll;

	/*改变transform的角度将引发矩阵的修改，该修改会延迟到下一帧或下一帧之后*/
	DQuaterion::Euler(&m_rotation, m_euler);
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

void DTransform::SetEuler(DVector3 euler)
{
	if (IS_FLOAT_EQUAL(m_euler.x, euler.x) && IS_FLOAT_EQUAL(m_euler.y, euler.y) && IS_FLOAT_EQUAL(m_euler.z, euler.z))
		return;
	m_euler = euler;

	/*改变transform的角度将引发矩阵的修改，该修改会延迟到下一帧或下一帧之后*/
	DQuaterion::Euler(&m_rotation, m_euler);
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

void DTransform::SetScale(float x, float y, float z)
{
	if (IS_FLOAT_EQUAL(m_scale.x, x) && IS_FLOAT_EQUAL(m_scale.y, y) && IS_FLOAT_EQUAL(m_scale.z, z))
		return;
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

void DTransform::SetScale(DVector3 scale)
{
	if (IS_FLOAT_EQUAL(m_scale.x, scale.x) && IS_FLOAT_EQUAL(m_scale.y, scale.y) && IS_FLOAT_EQUAL(m_scale.z, scale.z))
		return;
	m_scale = scale;
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
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
		//访问欧拉角时如果此前修改过四元数，则更新欧拉角
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
	if (m_isL2WMatrixChanged) {
		RefreshLocalToWorldMatrix();
	}
	up = m_up;
}

void DTransform::GetForward(DVector3 & forward)
{
	if (m_isL2WMatrixChanged) {
		RefreshLocalToWorldMatrix();
	}
	forward = m_forward;
}

void DTransform::GetRight(DVector3 & right)
{
	if (m_isL2WMatrixChanged) {
		RefreshLocalToWorldMatrix();
	}
	right = m_right;
	//DVector3::Cross(m_up, m_forward, right);
}

void DTransform::GetLocalToWorld(DMatrix4x4 & localToWorld)
{
	if (!m_isL2WMatrixChanged) {
		localToWorld = m_localToWorld;
		return;
	}

	RefreshLocalToWorldMatrix();

	localToWorld = m_localToWorld;
}

void DTransform::GetWorldToLocal(DMatrix4x4 & worldToLocal)
{
	if (!m_isW2LMatrixChanged)
	{
		worldToLocal = m_worldToLocal;
		return;
	}

	RefreshWorldToLocalMatrix();

	worldToLocal = m_worldToLocal;
}

void DTransform::TransformPointToWorld(const DVector3 & point, DVector3 & out)
{
	DMatrix4x4 toworld;
	GetLocalToWorld(toworld);

	toworld.TransformPoint(point, out);
}

void DTransform::TransformPointToLocal(const DVector3 & point, DVector3 & out)
{
	DMatrix4x4 tolocal;
	GetWorldToLocal(tolocal);

	tolocal.TransformPoint(point, out);
}

DSceneObject * DTransform::GetSceneObject()
{
	return m_sceneObj;
}

void DTransform::SetParent(DTransform * parent, bool useLocalInfo)
{
	if (m_parent != NULL)
	{
		//如果当前transform的父节点存在，则先从父节点移除
		RemoveFromParent();
	}
	m_parent = parent;

	//插入到父节点的子节点链表
	m_nextNeighbor = parent->m_firstChild;
	parent->m_firstChild = this;
	//父节点孩子数量+1
	parent->m_childCount += 1;

	//由于设置了父节点，导致矩阵发生变化
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

DTransform * DTransform::GetParent()
{
	if (m_parent != NULL && m_parent->m_isRoot) //如果父节点为场景根节点，则不允许返回该父节点
		return NULL;
	return m_parent;
}

void DTransform::RemoveFromParent()
{
	if (m_parent == NULL)
		return;
	
	//从父节点的孩子链表移除
	if (m_parent->m_firstChild == this)
	{
		m_parent->m_firstChild = this->m_nextNeighbor;
	}
	else if (m_preNeighbor != NULL)
	{
		m_preNeighbor->m_nextNeighbor = m_nextNeighbor;
	}

	//父节点孩子数量-1
	m_parent->m_childCount -= 1;
	m_preNeighbor = NULL;
	m_nextNeighbor = NULL;
	m_parent = NULL;

	//标记矩阵变化
	m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

unsigned int DTransform::GetChildCount()
{
	return m_childCount;
}

DTransform * DTransform::GetFirstChild()
{
	return m_firstChild;
}

DTransform * DTransform::GetNextNegibhor()
{
	return m_nextNeighbor;
}

bool DTransform::IsMatrixWillChange()
{
	return m_isL2WMatrixChanged;
}

void DTransform::Release()
{
	m_sceneObj = NULL;
	m_firstChild = NULL;
	m_nextNeighbor = NULL;
	m_parent = NULL;
}

void DTransform::RefreshLocalToWorldMatrix()
{
	m_isL2WMatrixChanged = false;

	DMatrix4x4::TRS(&m_localToWorld, &m_right, &m_up, &m_forward, m_position, m_rotation, m_scale);
	//m_worldToLocal = m_localToWorld

	if (m_parent != NULL && m_parent->m_isRoot == false) //不需要从根节点计算矩阵
	{
		DMatrix4x4 parentL2W;
		m_parent->GetLocalToWorld(parentL2W);
		m_localToWorld = m_localToWorld*parentL2W;

		DVector3 pright, pup, pforward;
		m_parent->GetRight(pright);
		m_parent->GetUp(pup);
		m_parent->GetForward(pforward);

		DVector3 nright = DVector3(m_right.x*pright.x + m_right.y*pup.x + m_right.z*pforward.x, m_right.x*pright.y + m_right.y*pup.y + m_right.z*pforward.y, m_right.x*pright.z + m_right.y*pup.z + m_right.z*pforward.z);
		DVector3 nup = DVector3(m_up.x*pright.x + m_up.y*pup.x + m_up.z*pforward.x, m_up.x*pright.y + m_up.y*pup.y + m_up.z*pforward.y, m_up.x*pright.z + m_up.y*pup.z + m_up.z*pforward.z);
		DVector3 nforward = DVector3(m_forward.x*pright.x + m_forward.y*pup.x + m_forward.z*pforward.x, m_forward.x*pright.y + m_forward.y*pup.y + m_forward.z*pforward.y, m_forward.x*pright.z + m_forward.y*pup.z + m_forward.z*pforward.z);

		m_right = nright;
		m_up = nup;
		m_forward = nforward;
	}

	//矩阵发生变化时将子节点也标记为矩阵变化
	DTransform* node = m_firstChild;
	while (node != NULL)
	{
		node->m_isL2WMatrixChanged = true;
		node = node->m_nextNeighbor;
	}
}

void DTransform::RefreshWorldToLocalMatrix()
{
	m_isW2LMatrixChanged = false;
	if (m_isL2WMatrixChanged)
	{
		RefreshLocalToWorldMatrix();
	}
	DMatrix4x4::Inverse(&m_worldToLocal, m_localToWorld);
}

void DTransform::RefreshEuler()
{
	m_rotation.EulerAngle(m_euler);
}

