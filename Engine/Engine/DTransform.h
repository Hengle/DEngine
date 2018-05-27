#pragma once
#include "DMath.h"

class DTransform {
public:
	DTransform();
	~DTransform();
	void SetPosition(FLOAT, FLOAT, FLOAT);
	void SetPosition(DVector3);
	void SetRotation(FLOAT, FLOAT, FLOAT, FLOAT);
	void SetRotation(DQuaterion);
	void SetEuler(FLOAT, FLOAT, FLOAT);
	void SetEuler(DVector3);
	void SetScale(FLOAT, FLOAT, FLOAT);
	void SetScale(DVector3);

	void GetPosition(DVector3&);
	void GetRotation(DQuaterion&);
	void GetEuler(DVector3&);
	void GetScale(DVector3&);
	void GetUp(DVector3&);
	void GetForward(DVector3&);
	void GetRight(DVector3&);
	void GetLocalToWorld(DMatrix4x4&);

	bool IsMatrixWillChange();

private:
	void RefreshMatrix();
	void RefreshEuler();
private:
	DVector3 m_position;
	DVector3 m_euler;
	DVector3 m_scale;
	DVector3 m_up;
	DVector3 m_forward;
	DQuaterion m_rotation;

	bool m_isMatrixChanged;
	bool m_isEulerChanged;

	DMatrix4x4 m_localToWorld;
};