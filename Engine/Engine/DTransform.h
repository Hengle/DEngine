﻿#pragma once
#include "DMath.h"
#include "DSceneObject.h"

class DSceneObject;

/*transform类*/
class DTransform
{
private:
	enum SpaceDefine
	{
		SpaceDefine_None = 0,
		SpaceDefine_Local = 1,
		SpaceDefine_World = 2,
	};

public:
	DTransform();
	DTransform(bool);
	DTransform(DSceneObject* data);
	~DTransform();
	void SetPosition(float, float, float);
	void SetPosition(DVector3);
	void SetLocalPosition(float, float, float);
	void SetLocalPosition(DVector3);
	void SetRotation(float, float, float, float);
	void SetRotation(DQuaterion);
	void SetLocalRotation(float, float, float, float);
	void SetLocalRotation(DQuaterion);
	void SetEuler(float, float, float);
	void SetEuler(DVector3);
	void SetLocalEuler(float, float, float);
	void SetLocalEuler(DVector3);
	void SetLocalScale(float, float, float);
	void SetLocalScale(DVector3);

	void GetPosition(DVector3&);
	void GetRotation(DQuaterion&);
	void GetEuler(DVector3&);
	void GetLocalPosition(DVector3&);
	void GetLocalRotation(DQuaterion&);
	void GetLocalEuler(DVector3&);
	void GetLocalScale(DVector3&);
	void GetLossyScale(DVector3&);
	void GetUp(DVector3&);
	void GetForward(DVector3&);
	void GetRight(DVector3&);
	void GetLocalToWorld(DMatrix4x4&);
	void GetWorldToLocal(DMatrix4x4&);
	void TransformPointToWorld(const DVector3 & point, DVector3 & out);
	void TransformPointToLocal(const DVector3 & point, DVector3 & out);

	DSceneObject* GetSceneObject();

	void SetParent(DTransform* parent, bool useLocalInfo = false);
	DTransform* GetParent();
	void RemoveFromParent();

	unsigned int GetChildCount();
	DTransform* GetFirstChild();
	DTransform* GetNextNegibhor();

	/*当前矩阵是否即将改变
	矩阵的改变只在下一帧访问该矩阵时才会更新，以避免每次修改transform都导致矩阵刷新*/
	bool IsMatrixWillChange();

	void Release();

private:
	void RefreshLocalToWorldMatrix();
	void RefreshWorldToLocalMatrix();
	void RefreshEuler();
	void RefreshQuaterion();

	//void RefreshWorldTransform();

private:
	DVector3 m_position;
	DVector3 m_localPosition;
	DVector3 m_euler;
	DVector3 m_localEuler;
	DVector3 m_localScale;
	DVector3 m_lossyScale;
	DVector3 m_right;
	DVector3 m_up;
	DVector3 m_forward;
	DQuaterion m_rotation;
	DQuaterion m_localRotation;

	DTransform* m_parent;
	DTransform* m_preNeighbor;
	DTransform* m_nextNeighbor;

	DTransform* m_firstChild;
	int m_childCount;

	//bool m_isL2WMatrixChanged;
	//bool m_isW2LMatrixChanged;
	//bool m_isEulerChanged;
	//bool m_isLocalTransformChanged; //标记本地transform信息是否变化，该变化会引起重新计算
	//bool m_isWorldTransformChanged;
	//bool m_isLocalEulerChanged;
	//bool m_isWorldEulerChanged;
	//SpaceDefine m_transformChangeMark; //标记当前引起transform变化的信息来自本地还是世界
	//SpaceDefine m_eullerChangeMark;  //标记引起当前角度变化的信息来自本地还是世界
	//bool m_isRotationChanged;
	bool m_isW2LMatrixChanged;    //标记worldToLocal矩阵是否发生变化

	SpaceDefine m_positionChangedMark; //标记当前坐标改变的来源-可能设置局部或世界坐标，根据来源不同，矩阵的更新也不同
	SpaceDefine m_eulerChangedMark;  //标记当前欧拉角改变的来源-可能设置局部或世界欧拉角，根据来源不同，矩阵的更新也不同
	SpaceDefine m_rotationChangedMark;  
	bool m_isLocalScaleChanged;

	DMatrix4x4 m_localToWorld;
	DMatrix4x4 m_worldToLocal;

	DSceneObject* m_sceneObj;

	bool m_isRoot;
};

