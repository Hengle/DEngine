#pragma once
#include "DMath.h"
#include "DSceneObject.h"

class DSceneObject;

/*transform类*/
class DTransform
{
public:
	DTransform();
	DTransform(bool);
	DTransform(DSceneObject* data);
	~DTransform();
	void SetPosition(float, float, float);
	void SetPosition(DVector3);
	void SetRotation(float, float, float, float);
	void SetRotation(DQuaterion);
	void SetEuler(float, float, float);
	void SetEuler(DVector3);
	void SetScale(float, float, float);
	void SetScale(DVector3);

	void GetPosition(DVector3&);
	void GetRotation(DQuaterion&);
	void GetEuler(DVector3&);
	void GetScale(DVector3&);
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
private:
	DVector3 m_position;
	DVector3 m_euler;
	DVector3 m_scale;
	DVector3 m_right;
	DVector3 m_up;
	DVector3 m_forward;
	DQuaterion m_rotation;

	DTransform* m_parent;
	DTransform* m_preNeighbor;
	DTransform* m_nextNeighbor;

	DTransform* m_firstChild;
	int m_childCount;

	bool m_isL2WMatrixChanged;
	bool m_isW2LMatrixChanged;
	bool m_isEulerChanged;

	DMatrix4x4 m_localToWorld;
	DMatrix4x4 m_worldToLocal;

	DSceneObject* m_sceneObj;

	bool m_isRoot;
};

