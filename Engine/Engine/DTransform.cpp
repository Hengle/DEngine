#include "DTransform.h"

DTransform::DTransform()
{
	m_position = DVector3(0, 0, 0);
	m_localPosition = DVector3(0, 0, 0);
	m_localScale = DVector3(1, 1, 1);
	//m_lossyScale = DVector3(1, 1, 1);
	m_right = DVector3(1, 0, 0);
	m_up = DVector3(0, 1, 0);
	m_forward = DVector3(0, 0, 1);
	m_rotation = DQuaternion(0, 0, 0, 1);
	m_localRotation = DQuaternion(0, 0, 0, 1);
	m_euler = DVector3(0, 0, 0);
	m_localEuler = DVector3(0, 0, 0);

	//m_isL2WMatrixChanged = false;
	//m_isW2LMatrixChanged = false;
	m_isMatrixChanged = false;
	m_positionChangedMark = SpaceDefine_None;
	m_eulerChangedMark = SpaceDefine_None;
	m_rotationChangedMark = SpaceDefine_None;
	//m_scaleChangedMark = SpaceDefine_None;

	m_childCount = 0;

	m_localToWorld = dmat_identity;
	m_worldToLocal = dmat_identity;

	m_sceneObj = NULL;

	m_parent = NULL;
	m_preNeighbor = NULL;
	m_nextNeighbor = NULL;

	m_firstChild = NULL;

	m_isRoot = false;
	m_isAreaChanged = false;

	//m_isParentChanged = false;
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
	/*DVector3 pos;
	GetPosition(pos);
	if (IS_FLOAT_EQUAL(pos.x, x) && IS_FLOAT_EQUAL(pos.y, y) && IS_FLOAT_EQUAL(pos.z, z))
		return;*/
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	//标记当前坐标改变，并且来源是世界坐标
	m_positionChangedMark = SpaceDefine_World;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetPosition(DVector3 position)
{
	/*DVector3 pos;
	GetPosition(pos);
	if (IS_FLOAT_EQUAL(pos.x, position.x) && IS_FLOAT_EQUAL(pos.y, position.y) && IS_FLOAT_EQUAL(pos.z, position.z))
		return;*/
	m_position = position;

	//标记当前坐标改变，并且来源是世界坐标
	m_positionChangedMark = SpaceDefine_World;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalPosition(float x, float y, float z)
{
	/*DVector3 localPos;
	GetLocalPosition(localPos);
	if (IS_FLOAT_EQUAL(localPos.x, x) && IS_FLOAT_EQUAL(localPos.y, y) && IS_FLOAT_EQUAL(localPos.z, z))
		return;*/
	m_localPosition.x = x;
	m_localPosition.y = y;
	m_localPosition.z = z;

	//标记当前坐标改变，并且来源是局部坐标
	m_positionChangedMark = SpaceDefine_Local;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalPosition(DVector3 localPosition)
{
	/*DVector3 localPos;
	GetLocalPosition(localPos);
	if (IS_FLOAT_EQUAL(localPos.x, localPosition.x) && IS_FLOAT_EQUAL(localPos.y, localPosition.y) && IS_FLOAT_EQUAL(localPos.z, localPosition.z))
		return;*/
	m_localPosition = localPosition;

	//标记当前坐标改变，并且来源是局部坐标
	m_positionChangedMark = SpaceDefine_Local;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetRotation(float x, float y, float z, float w)
{
	/*DQuaternion rot;
	GetRotation(rot);
	if (IS_FLOAT_EQUAL(rot.x, x) && IS_FLOAT_EQUAL(rot.y, y) && IS_FLOAT_EQUAL(rot.z, z) && IS_FLOAT_EQUAL(rot.w, w))
		return;*/
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
	m_rotation.w = w;

	//标记当前旋转改变，并且来源是世界旋转
	m_rotationChangedMark = SpaceDefine_World;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetRotation(DQuaternion rotation)
{
	/*DQuaternion rot;
	GetRotation(rot);
	if (IS_FLOAT_EQUAL(rot.x, rotation.x) && IS_FLOAT_EQUAL(rot.y, rotation.y) && IS_FLOAT_EQUAL(rot.z, rotation.z) && IS_FLOAT_EQUAL(rot.w, rotation.w))
		return;*/
	m_rotation = rotation;

	//标记当前旋转改变，并且来源是世界旋转
	m_rotationChangedMark = SpaceDefine_World;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalRotation(float x, float y, float z, float w)
{
	/*DQuaternion localrot;
	GetLocalRotation(localrot);
	if (IS_FLOAT_EQUAL(localrot.x, x) && IS_FLOAT_EQUAL(localrot.y, y) && IS_FLOAT_EQUAL(localrot.z, z) && IS_FLOAT_EQUAL(localrot.w, w))
		return;*/
	m_localRotation.x = x;
	m_localRotation.y = y;
	m_localRotation.z = z;
	m_localRotation.w = w;

	//标记当前旋转改变，并且来源是局部旋转
	m_rotationChangedMark = SpaceDefine_Local;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalRotation(DQuaternion localRotation)
{
	/*DQuaternion localrot;
	GetLocalRotation(localrot);
	if (IS_FLOAT_EQUAL(localrot.x, localRotation.x) && IS_FLOAT_EQUAL(localrot.y, localRotation.y) && IS_FLOAT_EQUAL(localrot.z, localRotation.z) && IS_FLOAT_EQUAL(localrot.w, localRotation.w))
		return;*/
	m_localRotation = localRotation;

	//标记当前旋转改变，并且来源是局部旋转
	m_rotationChangedMark = SpaceDefine_Local;
	//由于设置了四元数，则取消欧拉角改变，避免引起重计算四元数
	m_eulerChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetEuler(float pitch, float yaw, float roll)
{
	/*DVector3 euler;
	GetEuler(euler);
	if (IS_FLOAT_EQUAL(euler.x, pitch) && IS_FLOAT_EQUAL(euler.y, yaw) && IS_FLOAT_EQUAL(euler.z, roll))
		return;*/
	m_euler.x = pitch;
	m_euler.y = yaw;
	m_euler.z = roll;

	//标记当前欧拉旋转改变，并且来源是世界旋转
	m_eulerChangedMark = SpaceDefine_World;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetEuler(DVector3 euler)
{
	/*DVector3 eu;
	GetEuler(eu);
	if (IS_FLOAT_EQUAL(eu.x, euler.x) && IS_FLOAT_EQUAL(eu.y, euler.y) && IS_FLOAT_EQUAL(eu.z, euler.z))
		return;*/
	m_euler = euler;

	//标记当前欧拉旋转改变，并且来源是世界旋转
	m_eulerChangedMark = SpaceDefine_World;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalEuler(float pitch, float yaw, float roll)
{
	/*DVector3 localeuler;
	GetLocalEuler(localeuler);
	if (IS_FLOAT_EQUAL(localeuler.x, pitch) && IS_FLOAT_EQUAL(localeuler.y, yaw) && IS_FLOAT_EQUAL(localeuler.z, roll))
		return;*/
	m_localEuler.x = pitch;
	m_localEuler.y = yaw;
	m_localEuler.z = roll;

	//标记当前欧拉旋转改变，并且来源是局部旋转
	m_eulerChangedMark = SpaceDefine_Local;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalEuler(DVector3 localEuler)
{
	/*DVector3 leuler;
	GetLocalEuler(leuler);
	if (IS_FLOAT_EQUAL(leuler.x, localEuler.x) && IS_FLOAT_EQUAL(leuler.y, localEuler.y) && IS_FLOAT_EQUAL(leuler.z, localEuler.z))
		return;*/
	m_localEuler = localEuler;

	//标记当前欧拉旋转改变，并且来源是局部旋转
	m_eulerChangedMark = SpaceDefine_Local;
	//由于设置了欧拉角，则取消四元数改变，避免引起重计算欧拉角
	m_rotationChangedMark = SpaceDefine_None;
	//标记矩阵改变
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalScale(float x, float y, float z)
{
	/*DVector3 localScale;
	GetLocalScale(localScale);
	if (IS_FLOAT_EQUAL(localScale.x, x) && IS_FLOAT_EQUAL(localScale.y, y) && IS_FLOAT_EQUAL(localScale.z, z))
		return;*/
	m_localScale.x = x;
	m_localScale.y = y;
	m_localScale.z = z;
	
	//m_scaleChangedMark = SpaceDefine_Local;
	m_isMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	SetChildChange(this);
}

void DTransform::SetLocalScale(DVector3 localScale)
{
	//if (IS_FLOAT_EQUAL(m_localScale.x, localScale.x) && IS_FLOAT_EQUAL(m_localScale.y, localScale.y) && IS_FLOAT_EQUAL(m_localScale.z, localScale.z))
	//	return;
	m_localScale = localScale;
	
	//m_scaleChangedMark = SpaceDefine_Local;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isAreaChanged = true;
	m_isMatrixChanged = true;
	SetChildChange(this);
}

void DTransform::GetPosition(DVector3 &position)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if(m_isParentChanged)
	//{
	//	RefreshLocalToWorldMatrix();
	//}
	//else if (m_positionChangedMark == SpaceDefine_Local)
	//{
	//	//更新坐标
	//	RefreshPosition();
	//}

	position = m_position;
}

void DTransform::GetRotation(DQuaternion & rotation)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isParentChanged)
	//{
	//	RefreshLocalToWorldMatrix();
	//}
	//else if (m_rotationChangedMark == SpaceDefine_Local  /*上一次设置了局部四元数改变*/
	//	|| m_eulerChangedMark != SpaceDefine_None  /*上一次设置了欧拉角改变*/ )
	//{
	//	//更新坐标
	//	RefreshRotation();
	//}

	rotation = m_rotation;
}

void DTransform::GetEuler(DVector3 & euler)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isParentChanged)
	//{
	//	RefreshLocalToWorldMatrix();
	//}
	//else if (m_eulerChangedMark == SpaceDefine_Local  /*上一次设置了局部欧拉角改变*/
	//	|| m_rotationChangedMark != SpaceDefine_None  /*上一次设置了四元数改变*/)
	//{
	//	RefreshRotation();
	//}
	euler = m_euler;
}

void DTransform::GetLocalPosition(DVector3 & localPosition)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	/*if (m_isParentChanged)
	{
		RefreshLocalToWorldMatrix();
	}
	else if (m_positionChangedMark == SpaceDefine_World)
	{
		RefreshPosition();
	}*/

	localPosition = m_localPosition;
}

void DTransform::GetLocalRotation(DQuaternion & localRotation)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isParentChanged)
	//{
	//	RefreshLocalToWorldMatrix();
	//}
	//else if (m_rotationChangedMark == SpaceDefine_World  /*上一次设置了世界四元数改变*/
	//	|| m_eulerChangedMark != SpaceDefine_None  /*上一次设置了欧拉角改变*/)
	//{
	//	RefreshRotation();
	//}

	localRotation = m_localRotation;
}

void DTransform::GetLocalEuler(DVector3 & localEuler)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isParentChanged)
	//{
	//	RefreshLocalToWorldMatrix();
	//}
	//else if (m_eulerChangedMark == SpaceDefine_World  /*上一次设置了世界欧拉角改变*/
	//	|| m_rotationChangedMark != SpaceDefine_None  /*上一次设置了四元数改变*/)
	//{
	//	RefreshRotation();
	//}

	localEuler = m_localEuler;
}

void DTransform::GetLocalScale(DVector3 & localScale)
{
	/*if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}*/
	/*if (m_isParentChanged)
	{
		RefreshLocalToWorldMatrix();
	}
	else if (m_scaleChangedMark == SpaceDefine_World)
	{
		RefreshScale();
	}*/
	localScale = m_localScale;
}

void DTransform::GetLossyScale(DVector3 & lossyScale)
{
	/*if (m_isParentChanged)
	{
		RefreshLocalToWorldMatrix();
	}
	else if (m_scaleChangedMark == SpaceDefine_Local)
	{
		RefreshScale();
	}*/
	DTransform* parent = GetParent();
	if (parent != NULL) {
		DVector3 scale;
		parent->GetLossyScale(scale);
		lossyScale = DVector3(scale.x*m_localScale.x, scale.y*m_localScale.y, scale.z*m_localScale.z);
	}
	else
	{
		lossyScale = m_localScale;
	}
	//lossyScale = m_lossyScale;
}

void DTransform::GetUp(DVector3 & up)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isL2WMatrixChanged || m_isParentChanged) { //方向向量仅取决于旋转，因此当发生旋转时更新矩阵的旋转部分
	//	RefreshLocalToWorldMatrix();
	//}
	up = m_up;
}

void DTransform::GetForward(DVector3 & forward)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isL2WMatrixChanged || m_isParentChanged) { //方向向量仅取决于旋转，因此当发生旋转时更新矩阵的旋转部分
	//	RefreshLocalToWorldMatrix();
	//}
	forward = m_forward;
}

void DTransform::GetRight(DVector3 & right)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isL2WMatrixChanged || m_isParentChanged) { //方向向量仅取决于旋转，因此当发生旋转时更新矩阵的旋转部分
	//	RefreshLocalToWorldMatrix();
	//}
	right = m_right;
}

void DTransform::GetLocalToWorld(DMatrix4x4 & localToWorld)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	//if (m_isL2WMatrixChanged || m_isParentChanged) {  //存在任何引起矩阵变化的元素都重新计算矩阵
	//	RefreshLocalToWorldMatrix();
	//}

	localToWorld = m_localToWorld;
}

void DTransform::GetWorldToLocal(DMatrix4x4 & worldToLocal)
{
	if (m_isMatrixChanged)
	{
		RefreshMatrix();
	}
	/*if (m_isW2LMatrixChanged || m_isParentChanged)
	{
		RefreshWorldToLocalMatrix();
	}*/

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
	DVector3 lossyScale;
	GetLossyScale(lossyScale);
	if (m_parent != NULL)
	{
		//如果当前transform的父节点存在，则先从父节点移除
		RemoveFromParent();
	}
	m_parent = parent;

	//插入到父节点的子节点链表
	m_nextNeighbor = parent->m_firstChild;
	if (parent->m_firstChild != NULL)
		parent->m_firstChild->m_preNeighbor = this;
	m_preNeighbor = NULL;
	parent->m_firstChild = this;
	//父节点孩子数量+1
	parent->m_childCount += 1;

	//由于设置了父节点，导致矩阵发生变化
	if (useLocalInfo) 
	{
		m_positionChangedMark = SpaceDefine_Local;
		m_rotationChangedMark = SpaceDefine_Local;
		//m_scaleChangedMark = SpaceDefine_Local;
	}
	else
	{
		

		m_positionChangedMark = SpaceDefine_World;
		m_rotationChangedMark = SpaceDefine_World;
		SetLossyScale(lossyScale.x, lossyScale.y, lossyScale.z);
		//m_scaleChangedMark = SpaceDefine_World;
		//m_lossyScale = m_localScale;
	}

	/*RefreshPosition();
	RefreshRotation();
	RefreshScale();*/

	//RefreshScale(useLocalInfo);

	//RefreshMatrix();

	SetChildChange(this);

	//m_isW2LMatrixChanged = true;
	//m_isW2LMatrixChanged = true;
	//m_isL2WMatrixChanged = true;
	m_isMatrixChanged = true;
	m_isAreaChanged = true;
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
		this->m_nextNeighbor->m_preNeighbor = NULL;
	}
	else if (m_preNeighbor != NULL)
	{
		m_preNeighbor->m_nextNeighbor = m_nextNeighbor;
		m_nextNeighbor->m_preNeighbor = m_preNeighbor;
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

bool DTransform::IsAreaChanged()
{
	return m_isAreaChanged;
}

void DTransform::ClearAreaChange()
{
	m_isAreaChanged = false;
}

//bool DTransform::IsMatrixWillChange()
//{
//	return m_isL2WMatrixChanged;
//}

void DTransform::Release()
{
	m_sceneObj = NULL;
	m_firstChild = NULL;
	m_nextNeighbor = NULL;
	m_parent = NULL;
}

void DTransform::SetChildChange(DTransform* transform)
{
	DTransform* node = transform->m_firstChild;
	while (node != NULL)
	{
		node->m_isMatrixChanged = true;
		node->m_isAreaChanged = true;
		//node->m_isParentChanged = true;
		SetChildChange(node);
		node = node->m_nextNeighbor;
	}
}

void DTransform::SetLossyScale(float x, float y, float z)
{
	//if (IS_FLOAT_EQUAL(m_lossyScale.x, x) && IS_FLOAT_EQUAL(m_lossyScale.y, y) && IS_FLOAT_EQUAL(m_lossyScale.z, z))
	//	return;
	/*m_lossyScale.x = x;
	m_lossyScale.y = y;
	m_lossyScale.z = z;

	m_scaleChangedMark = SpaceDefine_World;
	m_isW2LMatrixChanged = true;
	m_isL2WMatrixChanged = true;*/

	//SetChildChange(this);
	DTransform* parent = GetParent();
	if (parent != NULL)
	{
		DVector3 pscale;
		parent->GetLossyScale(pscale);
		
		if (IS_FLOAT_EQUAL(pscale.x, 0.0f))
			m_localScale.x = 0.0f;
		else
			m_localScale.x = x / pscale.x;
		if (IS_FLOAT_EQUAL(pscale.y, 0.0f))
			m_localScale.y = 0.0f;
		else
			m_localScale.y = y / pscale.y;
		if (IS_FLOAT_EQUAL(pscale.z, 0.0f))
			m_localScale.z = 0.0f;
		else
			m_localScale.z = z / pscale.z;
	}
	else
	{
		m_localScale = DVector3(x, y, z);
	}
}

void DTransform::RefreshLocalToWorldMatrix()
{
	/*RefreshPosition();
	RefreshRotation();
	RefreshScale();*/

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

	DTransform* parent = GetParent();

	if (parent) //不需要从根节点计算矩阵
	{
		DMatrix4x4 parentL2W;
		parent->GetLocalToWorld(parentL2W);
		m_localToWorld = m_localToWorld*parentL2W;

		DVector3 pright, pup, pforward, pscale;
		parent->GetRight(pright);
		parent->GetUp(pup);
		parent->GetForward(pforward);
		parent->GetLocalScale(pscale);

		DVector3 nright = DVector3(m_right.x*pright.x + m_right.y*pup.x + m_right.z*pforward.x, m_right.x*pright.y + m_right.y*pup.y + m_right.z*pforward.y, m_right.x*pright.z + m_right.y*pup.z + m_right.z*pforward.z);
		DVector3 nup = DVector3(m_up.x*pright.x + m_up.y*pup.x + m_up.z*pforward.x, m_up.x*pright.y + m_up.y*pup.y + m_up.z*pforward.y, m_up.x*pright.z + m_up.y*pup.z + m_up.z*pforward.z);
		DVector3 nforward = DVector3(m_forward.x*pright.x + m_forward.y*pup.x + m_forward.z*pforward.x, m_forward.x*pright.y + m_forward.y*pup.y + m_forward.z*pforward.y, m_forward.x*pright.z + m_forward.y*pup.z + m_forward.z*pforward.z);

		m_right = nright;
		m_up = nup;
		m_forward = nforward;
	}

	/*DTransform* node = m_firstChild;
	while (node != NULL)
	{
		node->m_isL2WMatrixChanged = true;
		node->m_isAreaChanged = true;
		node = node->m_nextNeighbor;
	}*/

	
	//m_isL2WMatrixChanged = false;
	//m_isParentChanged = false;
}

void DTransform::RefreshWorldToLocalMatrix()
{
	/*m_isW2LMatrixChanged = false;
	if (m_isL2WMatrixChanged)
	{
		RefreshLocalToWorldMatrix();
	}*/
	DMatrix4x4::Inverse(&m_worldToLocal, m_localToWorld);
}

void DTransform::RefreshMatrix()
{
	RefreshPosition();
	RefreshRotation();

	RefreshLocalToWorldMatrix();
	RefreshWorldToLocalMatrix();

	m_isMatrixChanged = false;
}

void DTransform::RefreshPosition()
{
	DTransform* parent = GetParent();
	if (parent != NULL)
	{
		if (m_positionChangedMark == SpaceDefine_Local)
		{
			//坐标更改来源为局部坐标，则根据父节点更新世界坐标
			parent->TransformPointToWorld(m_localPosition, m_position);
		}
		else if(m_positionChangedMark == SpaceDefine_World)
		{
			//坐标更改来源为世界坐标，则根据父节点更新局部坐标
			parent->TransformPointToLocal(m_position, m_localPosition);
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

//void DTransform::RefreshScale(bool useLocalInfo)
//{
//	if (m_parent != NULL)
//	{
//		DVector3 pscale;
//		m_parent->GetLossyScale(pscale);
//		if (!useLocalInfo)
//			m_localScale = DVector3(m_lossyScale.x / pscale.x, m_lossyScale.y / pscale.y, m_lossyScale.z / pscale.z);
//		}
//	}
//	else
//	{
//		if (m_scaleChangedMark == SpaceDefine_Local)
//		{
//			m_lossyScale = m_localScale;
//		}
//		else if (m_scaleChangedMark == SpaceDefine_World)
//		{
//			m_localScale = m_lossyScale;
//		}
//	}
//	m_scaleChangedMark = SpaceDefine_None;
//}

void DTransform::RefreshRotation()
{
	if (m_rotationChangedMark != SpaceDefine_None)
		RefreshEuler();
	if (m_eulerChangedMark != SpaceDefine_None)
		RefreshQuaterion();
}

void DTransform::RefreshEuler()
{
	DTransform* parent = GetParent();
	if (m_rotationChangedMark == SpaceDefine_Local) //局部四元数导致局部欧拉角发生变化
	{
		m_localRotation.EulerAngle(m_localEuler);
		if (parent != NULL)
		{
			DQuaternion protation;
			parent->GetLocalRotation(protation);
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
		if (parent != NULL)
		{
			DQuaternion protation,protationinv;
			parent->GetRotation(protation);
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
	DTransform* parent = GetParent();
	if (m_eulerChangedMark == SpaceDefine_Local)
	{
		DQuaternion::Euler(&m_localRotation, m_localEuler);
		if (parent != NULL)
		{
			DQuaternion protation;
			parent->GetLocalRotation(protation);
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
		if (parent != NULL)
		{
			DQuaternion protation, protationinv;
			parent->GetRotation(protation);
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

