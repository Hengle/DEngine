#include "DTransform.h"

DTransform::DTransform()
{
	m_position = DVector3(0, 0, 0);
	m_localPosition = DVector3(0, 0, 0);
	m_localScale = DVector3(1, 1, 1);
	m_lossyScale = DVector3(1, 1, 1);
	m_right = DVector3(1, 0, 0);
	m_up = DVector3(0, 1, 0);
	m_forward = DVector3(0, 0, 1);
	m_rotation = DQuaternion(0, 0, 0, 1);
	m_localRotation = DQuaternion(0, 0, 0, 1);
	m_euler = DVector3(0, 0, 0);
	m_localEuler = DVector3(0, 0, 0);

	m_isL2WMatrixChanged = false;
	m_isW2LMatrixChanged = false;
	m_positionChangedMark = SpaceDefine_None;
	m_eulerChangedMark = SpaceDefine_None;
	m_rotationChangedMark = SpaceDefine_None;
	m_scaleChangedMark = SpaceDefine_None;

	m_childCount = 0;

	m_localToWorld = dmat_identity;
	m_worldToLocal = dmat_identity;

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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::SetPosition(DVector3 position)
{
	if (IS_FLOAT_EQUAL(m_position.x, position.x) && IS_FLOAT_EQUAL(m_position.y, position.y) && IS_FLOAT_EQUAL(m_position.z, position.z))
		return;
	m_position = position;

	//标记当前坐标改变，并且来源是世界坐标
	m_positionChangedMark = SpaceDefine_World;
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::SetLocalPosition(DVector3 localPosition)
{
	if (IS_FLOAT_EQUAL(m_localPosition.x, localPosition.x) && IS_FLOAT_EQUAL(m_localPosition.y, localPosition.y) && IS_FLOAT_EQUAL(m_localPosition.z, localPosition.z))
		return;
	m_localPosition = localPosition;

	//标记当前坐标改变，并且来源是局部坐标
	m_positionChangedMark = SpaceDefine_Local;
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::SetRotation(DQuaternion rotation)
{
	if (IS_FLOAT_EQUAL(m_rotation.x, rotation.x) && IS_FLOAT_EQUAL(m_rotation.y, rotation.y) && IS_FLOAT_EQUAL(m_rotation.z, rotation.z) && IS_FLOAT_EQUAL(m_rotation.w, rotation.w))
		return;
	m_rotation = rotation;

	//标记当前旋转改变，并且来源是世界旋转
	m_rotationChangedMark = SpaceDefine_World;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::SetLocalRotation(DQuaternion localRotation)
{
	if (IS_FLOAT_EQUAL(m_localRotation.x, localRotation.x) && IS_FLOAT_EQUAL(m_localRotation.y, localRotation.y) && IS_FLOAT_EQUAL(m_localRotation.z, localRotation.z) && IS_FLOAT_EQUAL(m_localRotation.w, localRotation.w))
		return;
	m_localRotation = localRotation;

	//标记当前旋转改变，并且来源是局部旋转
	m_rotationChangedMark = SpaceDefine_Local;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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
	//标记矩阵改变
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::SetLocalScale(float x, float y, float z)
{
	if (IS_FLOAT_EQUAL(m_localScale.x, x) && IS_FLOAT_EQUAL(m_localScale.y, y) && IS_FLOAT_EQUAL(m_localScale.z, z))
		return;
	m_localScale.x = x;
	m_localScale.y = y;
	m_localScale.z = z;
	
	m_scaleChangedMark = SpaceDefine_Local;
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::SetLocalScale(DVector3 localScale)
{
	if (IS_FLOAT_EQUAL(m_localScale.x, localScale.x) && IS_FLOAT_EQUAL(m_localScale.y, localScale.y) && IS_FLOAT_EQUAL(m_localScale.z, localScale.z))
		return;
	m_localScale = localScale;
	
	m_scaleChangedMark = SpaceDefine_Local;
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::GetPosition(DVector3 &position)
{
	if (m_positionChangedMark == SpaceDefine_Local)
	{
		//更新坐标
		RefreshPosition();
	}

	position = m_position;
}

void DTransform::GetRotation(DQuaternion & rotation)
{
	if (m_rotationChangedMark == SpaceDefine_Local  /*上一次设置了局部四元数改变*/
		|| m_eulerChangedMark != SpaceDefine_None  /*上一次设置了欧拉角改变*/ )
	{
		//更新坐标
		RefreshRotation();
	}

	rotation = m_rotation;
}

void DTransform::GetEuler(DVector3 & euler)
{
	if (m_eulerChangedMark == SpaceDefine_Local  /*上一次设置了局部欧拉角改变*/
		|| m_rotationChangedMark != SpaceDefine_None  /*上一次设置了四元数改变*/)
	{
		RefreshRotation();
	}
	euler = m_euler;
}

void DTransform::GetLocalPosition(DVector3 & localPosition)
{
	if (m_positionChangedMark == SpaceDefine_World)
	{
		RefreshPosition();
	}

	localPosition = m_localPosition;
}

void DTransform::GetLocalRotation(DQuaternion & localRotation)
{
	if (m_rotationChangedMark == SpaceDefine_World  /*上一次设置了世界四元数改变*/
		|| m_eulerChangedMark != SpaceDefine_None  /*上一次设置了欧拉角改变*/)
	{
		RefreshRotation();
	}

	localRotation = m_localRotation;
}

void DTransform::GetLocalEuler(DVector3 & localEuler)
{
	if (m_eulerChangedMark == SpaceDefine_World  /*上一次设置了世界欧拉角改变*/
		|| m_rotationChangedMark != SpaceDefine_None  /*上一次设置了四元数改变*/)
	{
		RefreshRotation();
	}

	localEuler = m_localEuler;
}

void DTransform::GetLocalScale(DVector3 & localScale)
{
	if (m_scaleChangedMark == SpaceDefine_World)
	{
		RefreshScale();
	}
	localScale = m_localScale;
}

void DTransform::GetLossyScale(DVector3 & lossyScale)
{
	if (m_scaleChangedMark == SpaceDefine_Local)
	{
		RefreshScale();
	}
	lossyScale = m_lossyScale;
}

void DTransform::GetUp(DVector3 & up)
{
	if (m_isL2WMatrixChanged) { //方向向量仅取决于旋转，因此当发生旋转时更新矩阵的旋转部分
		RefreshLocalToWorldMatrix();
	}
	up = m_up;
}

void DTransform::GetForward(DVector3 & forward)
{
	if (m_isL2WMatrixChanged) { //方向向量仅取决于旋转，因此当发生旋转时更新矩阵的旋转部分
		RefreshLocalToWorldMatrix();
	}
	forward = m_forward;
}

void DTransform::GetRight(DVector3 & right)
{
	if (m_isL2WMatrixChanged) { //方向向量仅取决于旋转，因此当发生旋转时更新矩阵的旋转部分
		RefreshLocalToWorldMatrix();
	}
	right = m_right;
}

void DTransform::GetLocalToWorld(DMatrix4x4 & localToWorld)
{
	if (IsMatrixWillChange()) {  //存在任何引起矩阵变化的元素都重新计算矩阵
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

	RefreshPosition();
	RefreshRotation();
	RefreshScale();

	//由于设置了父节点，导致矩阵发生变化
	if (useLocalInfo) 
	{
		m_positionChangedMark = SpaceDefine_Local;
		m_rotationChangedMark = SpaceDefine_Local;
		m_scaleChangedMark = SpaceDefine_Local;
	}
	else
	{
		

		m_positionChangedMark = SpaceDefine_World;
		m_rotationChangedMark = SpaceDefine_World;
		m_scaleChangedMark = SpaceDefine_World;
		//m_lossyScale = m_localScale;
	}
	//m_isW2LMatrixChanged = true;
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
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

void DTransform::SetLossyScale(float x, float y, float z)
{
	if (IS_FLOAT_EQUAL(m_lossyScale.x, x) && IS_FLOAT_EQUAL(m_lossyScale.y, y) && IS_FLOAT_EQUAL(m_lossyScale.z, z))
		return;
	m_lossyScale.x = x;
	m_lossyScale.y = y;
	m_lossyScale.z = z;

	m_scaleChangedMark = SpaceDefine_World;
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;
}

void DTransform::RefreshLocalToWorldMatrix()
{
	RefreshPosition();
	RefreshRotation();
	RefreshScale();

	float x2 = 2.0f*m_localRotation.x*m_localRotation.x;
	float y2 = 2.0f*m_localRotation.y*m_localRotation.y;
	float z2 = 2.0f*m_localRotation.z*m_localRotation.z;
	float xy = 2.0f*m_localRotation.x*m_localRotation.y;
	float xz = 2.0f*m_localRotation.x*m_localRotation.z;
	float xw = 2.0f*m_localRotation.x*m_localRotation.w;
	float yz = 2.0f*m_localRotation.y*m_localRotation.z;
	float yw = 2.0f*m_localRotation.y*m_localRotation.w;
	float zw = 2.0f*m_localRotation.z*m_localRotation.w;
	float ra = 1.0f - y2 - z2;
	float rb = xy + zw;
	float rc = xz - yw;
	float rd = xy - zw;
	float re = 1.0f - x2 - z2;
	float rf = yz + xw;
	float rg = xz + yw;
	float rh = yz - xw;
	float ri = 1.0f - x2 - y2;

	m_right.x = ra;
	m_right.y = rb;
	m_right.z = rc;

	m_up.x = rd;
	m_up.y = re;
	m_up.z = rf;

	m_forward.x = rg;
	m_forward.y = rh;
	m_forward.z = ri;

	m_localToWorld.m00 = m_localScale.x*ra;
	m_localToWorld.m01 = m_localScale.x*rb;
	m_localToWorld.m02 = m_localScale.x*rc;
	m_localToWorld.m03 = 0.0f;
	m_localToWorld.m10 = m_localScale.y*rd;
	m_localToWorld.m11 = m_localScale.y*re;
	m_localToWorld.m12 = m_localScale.y*rf;
	m_localToWorld.m13 = 0.0f;
	m_localToWorld.m20 = m_localScale.z*rg;
	m_localToWorld.m21 = m_localScale.z*rh;
	m_localToWorld.m22 = m_localScale.z*ri;
	m_localToWorld.m23 = 0.0f;

	m_localToWorld.m30 = m_localPosition.x;
	m_localToWorld.m31 = m_localPosition.y;
	m_localToWorld.m32 = m_localPosition.z;
	m_localToWorld.m33 = 1.0f;

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
	}

	DTransform* node = m_firstChild;
	while (node != NULL)
	{
		node->m_isL2WMatrixChanged = true;
		node = node->m_nextNeighbor;
	}

	m_isL2WMatrixChanged = false;
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

void DTransform::RefreshPosition()
{
	if (m_parent != NULL)
	{
		if (m_positionChangedMark == SpaceDefine_Local)
		{
			//坐标更改来源为局部坐标，则根据父节点更新世界坐标
			m_parent->TransformPointToWorld(m_localPosition, m_position);
		}
		else if(m_positionChangedMark == SpaceDefine_World)
		{
			//坐标更改来源为世界坐标，则根据父节点更新局部坐标
			m_parent->TransformPointToLocal(m_position, m_localPosition);
		}
	}
	else
	{
		if (m_positionChangedMark == SpaceDefine_Local)
		{
			m_position = m_localPosition;
		}
		else if (m_positionChangedMark == SpaceDefine_World)
		{
			m_localPosition = m_position;
		}
	}
	m_positionChangedMark = SpaceDefine_None;
}

void DTransform::RefreshScale()
{
	if (m_parent != NULL)
	{
		DVector3 pscale;
		m_parent->GetLossyScale(pscale);
		if (m_scaleChangedMark == SpaceDefine_Local)
		{
			m_lossyScale = DVector3(pscale.x*m_localScale.x, pscale.y*m_localScale.y, pscale.z*m_localScale.z);
		}
		else if(m_scaleChangedMark == SpaceDefine_World)
		{
			m_localScale = DVector3(m_lossyScale.x / pscale.x, m_lossyScale.y / pscale.y, m_lossyScale.z / pscale.z);
		}
	}
	else
	{
		if (m_scaleChangedMark == SpaceDefine_Local)
		{
			m_lossyScale = m_localScale;
		}
		else if (m_scaleChangedMark == SpaceDefine_World)
		{
			m_localScale = m_lossyScale;
		}
	}
	m_scaleChangedMark = SpaceDefine_None;
}

void DTransform::RefreshRotation()
{
	if (m_rotationChangedMark != SpaceDefine_None)
		RefreshEuler();
	if (m_eulerChangedMark != SpaceDefine_None)
		RefreshQuaterion();
}

void DTransform::RefreshEuler()
{
	if (m_rotationChangedMark == SpaceDefine_Local) //局部四元数导致局部欧拉角发生变化
	{
		m_localRotation.EulerAngle(m_localEuler);
		if (m_parent != NULL)
		{
			DQuaternion protation;
			m_parent->GetLocalRotation(protation);
			m_rotation = protation*m_localRotation;
			m_rotation.EulerAngle(m_euler);
		}
		else
		{
			m_rotation = m_localRotation;
			m_rotation.EulerAngle(m_euler);
		}
	}
	else if (m_rotationChangedMark == SpaceDefine_World) //世界四元数导致世界欧拉角发生变化
	{
		m_rotation.EulerAngle(m_euler);
		if (m_parent != NULL)
		{
			DQuaternion protation,protationinv;
			m_parent->GetRotation(protation);
			DQuaternion::Inverse(protation, protationinv);
			m_localRotation = protationinv*m_rotation;
			m_localRotation.EulerAngle(m_localEuler);
		}
		else
		{
			m_localRotation = m_rotation;
			m_localRotation.EulerAngle(m_localEuler);
		}
	}
	m_rotationChangedMark = SpaceDefine_None;
}

void DTransform::RefreshQuaterion()
{
	if (m_eulerChangedMark == SpaceDefine_Local)
	{
		DQuaternion::Euler(&m_localRotation, m_localEuler);
		if (m_parent != NULL)
		{
			DQuaternion protation;
			m_parent->GetLocalRotation(protation);
			m_rotation = protation*m_localRotation;
			m_rotation.EulerAngle(m_euler);
		}
		else
		{
			m_rotation = m_localRotation;
			m_rotation.EulerAngle(m_euler);
		}
	}
	else if (m_eulerChangedMark == SpaceDefine_World)
	{
		DQuaternion::Euler(&m_rotation, m_euler);
		if (m_parent != NULL)
		{
			DQuaternion protation, protationinv;
			m_parent->GetRotation(protation);
			DQuaternion::Inverse(protation, protationinv);
			m_localRotation = protationinv*m_rotation;
			m_localRotation.EulerAngle(m_localEuler);
		}
		else
		{
			m_localRotation = m_rotation;
			m_localRotation.EulerAngle(m_localEuler);
		}
	}
	m_eulerChangedMark = SpaceDefine_None;
}

