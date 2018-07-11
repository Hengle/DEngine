#pragma once
#include "DMath.h"

class DTransform
{
public:
	DTransform();
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

	bool IsMatrixWillChange();

private:
	void RefreshLocalToWorldMatrix();
	void RefreshWorldToLocalMatrix();
	void RefreshEuler();
private:
	DVector3 m_position;
	DVector3 m_euler;
	DVector3 m_scale;
	DVector3 m_up;
	DVector3 m_forward;
	DQuaterion m_rotation;

	//DTransform* m_parent;
	//DTransform* 

	bool m_isL2WMatrixChanged;
	bool m_isW2LMatrixChanged;
	bool m_isEulerChanged;

	DMatrix4x4 m_localToWorld;
	DMatrix4x4 m_worldToLocal;
};

