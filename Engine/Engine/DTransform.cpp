#include "DTransform.h"

DTransform::DTransform()
{
	m_position = DVector3(0, 0, 0);
	m_localPosition = DVector3(0, 0, 0);
	//m_euler = D3DXVECTOR3();
	m_localScale = DVector3(1, 1, 1);
	m_right = DVector3(1, 0, 0);
	m_up = DVector3(0, 1, 0);
	m_forward = DVector3(0, 0, 1);
	m_rotation = DQuaterion(0, 0, 0, 1);
	m_localRotation = DQuaterion(0, 0, 0, 1);
	m_euler = DVector3(0, 0, 0);
	m_localEuler = DVector3(0, 0, 0);

	//m_isL2WMatrixChanged = false;
	//m_isW2LMatrixChanged = false;
	//m_isEulerChanged = false;
	//m_transformChangeMark = SpaceDefine_None;
	//m_eullerChangeMark = SpaceDefine_None;
	//m_isRotationChanged = false;
	m_isW2LMatrixChanged = false;
	m_positionChangedMark = SpaceDefine_None;
	m_eulerChangedMark = SpaceDefine_None;
	m_rotationChangedMark = SpaceDefine_None;
	m_isLocalScaleChanged = false;

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

	//标记当前坐标改变，并且来源是世界坐标
	m_positionChangedMark = SpaceDefine_World;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetPosition(DVector3 position)
{
	if (IS_FLOAT_EQUAL(m_position.x, position.x) && IS_FLOAT_EQUAL(m_position.y, position.y) && IS_FLOAT_EQUAL(m_position.z, position.z))
		return;
	m_position = position;

	//标记当前坐标改变，并且来源是世界坐标
	m_positionChangedMark = SpaceDefine_World;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalPosition(float x, float y, float z)
{
	if (IS_FLOAT_EQUAL(m_localPosition.x, x) && IS_FLOAT_EQUAL(m_localPosition.y, y) && IS_FLOAT_EQUAL(m_localPosition.z, z))
		return;
	m_localPosition.x = x;
	m_localPosition.y = y;
	m_localPosition.z = z;

	//标记当前坐标改变，并且来源是局部坐标
	m_positionChangedMark = SpaceDefine_Local;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalPosition(DVector3 localPosition)
{
	if (IS_FLOAT_EQUAL(m_localPosition.x, localPosition.x) && IS_FLOAT_EQUAL(m_localPosition.y, localPosition.y) && IS_FLOAT_EQUAL(m_localPosition.z, localPosition.z))
		return;
	m_localPosition = localPosition;

	//标记当前坐标改变，并且来源是局部坐标
	m_positionChangedMark = SpaceDefine_Local;
	//标记worldtolocal矩阵改变
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

	//标记当前旋转改变，并且来源是世界旋转
	m_rotationChangedMark = SpaceDefine_World;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetRotation(DQuaterion rotation)
{
	if (IS_FLOAT_EQUAL(m_rotation.x, rotation.x) && IS_FLOAT_EQUAL(m_rotation.y, rotation.y) && IS_FLOAT_EQUAL(m_rotation.z, rotation.z) && IS_FLOAT_EQUAL(m_rotation.w, rotation.w))
		return;
	m_rotation = rotation;

	//标记当前旋转改变，并且来源是世界旋转
	m_rotationChangedMark = SpaceDefine_World;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalRotation(float x, float y, float z, float w)
{
	if (IS_FLOAT_EQUAL(m_localRotation.x, x) && IS_FLOAT_EQUAL(m_localRotation.y, y) && IS_FLOAT_EQUAL(m_localRotation.z, z) && IS_FLOAT_EQUAL(m_localRotation.w, w))
		return;
	m_localRotation.x = x;
	m_localRotation.y = y;
	m_localRotation.z = z;
	m_localRotation.w = w;

	//标记当前旋转改变，并且来源是局部旋转
	m_rotationChangedMark = SpaceDefine_Local;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalRotation(DQuaterion localRotation)
{
	if (IS_FLOAT_EQUAL(m_localRotation.x, localRotation.x) && IS_FLOAT_EQUAL(m_localRotation.y, localRotation.y) && IS_FLOAT_EQUAL(m_localRotation.z, localRotation.z) && IS_FLOAT_EQUAL(m_localRotation.w, localRotation.w))
		return;
	m_localRotation = localRotation;

	//标记当前旋转改变，并且来源是局部旋转
	m_rotationChangedMark = SpaceDefine_Local;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetEuler(float pitch, float yaw, float roll)
{
	if (IS_FLOAT_EQUAL(m_euler.x, pitch) && IS_FLOAT_EQUAL(m_euler.y, yaw) && IS_FLOAT_EQUAL(m_euler.z, roll))
		return;
	m_euler.x = pitch;
	m_euler.y = yaw;
	m_euler.z = roll;

	//标记当前欧拉旋转改变，并且来源是世界旋转
	m_eulerChangedMark = SpaceDefine_World;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetEuler(DVector3 euler)
{
	if (IS_FLOAT_EQUAL(m_euler.x, euler.x) && IS_FLOAT_EQUAL(m_euler.y, euler.y) && IS_FLOAT_EQUAL(m_euler.z, euler.z))
		return;
	m_euler = euler;

	//标记当前欧拉旋转改变，并且来源是世界旋转
	m_eulerChangedMark = SpaceDefine_World;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalEuler(float pitch, float yaw, float roll)
{
	if (IS_FLOAT_EQUAL(m_localEuler.x, pitch) && IS_FLOAT_EQUAL(m_localEuler.y, yaw) && IS_FLOAT_EQUAL(m_localEuler.z, roll))
		return;
	m_localEuler.x = pitch;
	m_localEuler.y = yaw;
	m_localEuler.z = roll;

	//标记当前欧拉旋转改变，并且来源是局部旋转
	m_eulerChangedMark = SpaceDefine_Local;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalEuler(DVector3 localEuler)
{
	if (IS_FLOAT_EQUAL(m_localEuler.x, localEuler.x) && IS_FLOAT_EQUAL(m_localEuler.y, localEuler.y) && IS_FLOAT_EQUAL(m_localEuler.z, localEuler.z))
		return;
	m_localEuler = localEuler;

	//标记当前欧拉旋转改变，并且来源是局部旋转
	m_eulerChangedMark = SpaceDefine_Local;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记worldtolocal矩阵改变
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalScale(float x, float y, float z)
{
	if (IS_FLOAT_EQUAL(m_localScale.x, x) && IS_FLOAT_EQUAL(m_localScale.y, y) && IS_FLOAT_EQUAL(m_localScale.z, z))
		return;
	m_localScale.x = x;
	m_localScale.y = y;
	m_localScale.z = z;
	m_isLocalScaleChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

void DTransform::SetLocalScale(DVector3 localScale)
{
	if (IS_FLOAT_EQUAL(m_localScale.x, localScale.x) && IS_FLOAT_EQUAL(m_localScale.y, localScale.y) && IS_FLOAT_EQUAL(m_localScale.z, localScale.z))
		return;
	m_localScale = localScale;
	m_isLocalScaleChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isW2LMatrixChanged = true;
}

void DTransform::GetPosition(DVector3 &position)
{
	if (m_positionChangedMark == SpaceDefine_Local)
	{
		//由于上一次改变坐标设置的是局部坐标，则此次获取坐标时重计算矩阵，并更新世界坐标
		RefreshLocalToWorldMatrix();
	}

	position = m_position;
}

void DTransform::GetRotation(DQuaterion & rotation)
{
	if (m_rotationChangedMark == SpaceDefine_Local  /*上一次设置了局部四元数改变*/
		|| m_eulerChangedMark != SpaceDefine_None  /*上一次设置了欧拉角改变*/ )
	{
		RefreshLocalToWorldMatrix();
	}

	rotation = m_rotation;
}

void DTransform::GetEuler(DVector3 & euler)
{
	if (m_eulerChangedMark == SpaceDefine_Local  /*上一次设置了局部欧拉角改变*/
		|| m_rotationChangedMark != SpaceDefine_None  /*上一次设置了四元数改变*/)
	{
		RefreshLocalToWorldMatrix();
	}
	//if (m_isEulerChanged) { 
	//	//访问欧拉角时如果此前修改过四元数，则更新欧拉角
	//	RefreshEuler();
	//	m_isEulerChanged = false;
	//}
	euler = m_euler;
}

void DTransform::GetLocalPosition(DVector3 & localPosition)
{
	if (m_positionChangedMark == SpaceDefine_World)
	{
		//由于上一次改变坐标设置的是世界坐标，则此次获取坐标时重计算矩阵，并更新局部坐标
		RefreshLocalToWorldMatrix();
	}

	localPosition = m_localPosition;
}

void DTransform::GetLocalRotation(DQuaterion & localRotation)
{
	if (m_rotationChangedMark == SpaceDefine_World  /*上一次设置了世界四元数改变*/
		|| m_eulerChangedMark != SpaceDefine_None  /*上一次设置了欧拉角改变*/)
	{
		RefreshLocalToWorldMatrix();
	}

	localRotation = m_localRotation;
}

void DTransform::GetLocalEuler(DVector3 & localEuler)
{
	if (m_eulerChangedMark == SpaceDefine_World  /*上一次设置了世界欧拉角改变*/
		|| m_rotationChangedMark != SpaceDefine_None  /*上一次设置了四元数改变*/)
	{
		RefreshLocalToWorldMatrix();
	}

	localEuler = m_localEuler;
}

void DTransform::GetLocalScale(DVector3 & localScale)
{
	if (m_isLocalScaleChanged) {
		RefreshLocalToWorldMatrix();
	}
	localScale = m_localScale;
}

void DTransform::GetLossyScale(DVector3 & lossyScale)
{
	if (m_isLocalScaleChanged) {
		RefreshLocalToWorldMatrix();
	}
	lossyScale = m_lossyScale;
}

void DTransform::GetUp(DVector3 & up)
{
	if (IsMatrixWillChange()) { //矩阵更新则意味着up向量会更新
		RefreshLocalToWorldMatrix();
	}
	up = m_up;
}

void DTransform::GetForward(DVector3 & forward)
{
	if (IsMatrixWillChange()) {
		RefreshLocalToWorldMatrix();
	}
	forward = m_forward;
}

void DTransform::GetRight(DVector3 & right)
{
	if (IsMatrixWillChange()) {
		RefreshLocalToWorldMatrix();
	}
	right = m_right;
	//DVector3::Cross(m_up, m_forward, right);
}

void DTransform::GetLocalToWorld(DMatrix4x4 & localToWorld)
{
	if (IsMatrixWillChange()) {
		RefreshLocalToWorldMatrix();
	}

	localToWorld = m_localToWorld;
}

void DTransform::GetWorldToLocal(DMatrix4x4 & worldToLocal)
{
	if (m_isW2LMatrixChanged)
	{
		RefreshWorldToLocalMatrix();
	}

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
	//角度、坐标、缩放发生改变都会引起矩阵变化
	return m_positionChangedMark != SpaceDefine_None || m_rotationChangedMark != SpaceDefine_None || m_eulerChangedMark != SpaceDefine_None || m_isLocalScaleChanged;
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
	if (m_eulerChangedMark != SpaceDefine_None)
		RefreshEuler();
	else if (m_rotationChangedMark != SpaceDefine_None)
		RefreshQuaterion();



	m_isL2WMatrixChanged = false;

	DMatrix4x4::TRS(&m_localToWorld, &m_right, &m_up, &m_forward, m_localPosition, m_localRotation, m_localScale);
	//m_worldToLocal = m_localToWorld

	if (m_parent != NULL && m_parent->m_isRoot == false) //不需要从根节点计算矩阵
	{
		DMatrix4x4 parentL2W;
		m_parent->GetLocalToWorld(parentL2W);
		m_localToWorld = m_localToWorld*parentL2W;

		DVector3 pright, pup, pforward, pscale;
		m_parent->GetRight(pright);
		m_parent->GetUp(pup);
		m_parent->GetForward(pforward);
		m_parent->GetLocalScale(pscale);

		DVector3 nright = DVector3(m_right.x*pright.x + m_right.y*pup.x + m_right.z*pforward.x, m_right.x*pright.y + m_right.y*pup.y + m_right.z*pforward.y, m_right.x*pright.z + m_right.y*pup.z + m_right.z*pforward.z);
		DVector3 nup = DVector3(m_up.x*pright.x + m_up.y*pup.x + m_up.z*pforward.x, m_up.x*pright.y + m_up.y*pup.y + m_up.z*pforward.y, m_up.x*pright.z + m_up.y*pup.z + m_up.z*pforward.z);
		DVector3 nforward = DVector3(m_forward.x*pright.x + m_forward.y*pup.x + m_forward.z*pforward.x, m_forward.x*pright.y + m_forward.y*pup.y + m_forward.z*pforward.y, m_forward.x*pright.z + m_forward.y*pup.z + m_forward.z*pforward.z);

		m_right = nright;
		m_up = nup;
		m_forward = nforward;

		m_lossyScale = DVector3(m_localScale.x*pscale.x, m_localScale.y*pscale.y, m_localScale.z*pscale.z);
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
	if (IsMatrixWillChange())
	{
		RefreshLocalToWorldMatrix();
	}
	DMatrix4x4::Inverse(&m_worldToLocal, m_localToWorld);
}

void DTransform::RefreshEuler()
{
	if (m_rotationChangedMark == SpaceDefine_Local) //局部四元数导致局部欧拉角发生变化
		m_localRotation.EulerAngle(m_localEuler);
	else if (m_rotationChangedMark == SpaceDefine_World) //世界四元数导致世界欧拉角发生变化
		m_rotation.EulerAngle(m_euler);
	//m_rotation.EulerAngle(m_euler);
}

void DTransform::RefreshQuaterion()
{
	if (m_eulerChangedMark == SpaceDefine_Local)
		DQuaterion::Euler(&m_localRotation, m_localEuler);
	else if (m_eulerChangedMark == SpaceDefine_World)
		DQuaterion::Euler(&m_rotation, m_euler);
}

void DTransform::RefreshWorldTransform()
{
	DVector3 wpos = DVector3(m_localToWorld.m30, m_localToWorld.m31, m_localToWorld.m32);

	float ra = m_localToWorld.m00 / m_lossyScale.x;
	float rb = m_localToWorld.m01 / m_lossyScale.x;
	float rc = m_localToWorld.m02 / m_lossyScale.x;
	float rd = m_localToWorld.m10 / m_lossyScale.y;
	float re = m_localToWorld.m11 / m_lossyScale.y;
	float rf = m_localToWorld.m12 / m_lossyScale.y;
	float rg = m_localToWorld.m20 / m_lossyScale.z;
	float rh = m_localToWorld.m21 / m_lossyScale.z;
	float ri = m_localToWorld.m22 / m_lossyScale.z;
}

