#pragma once
#include "DMath.h"

class DTransform {
public:
	DTransform();
	~DTransform();
	void SetPosition(FLOAT, FLOAT, FLOAT);
	void SetPosition(D3DXVECTOR3);
	void SetRotation(FLOAT, FLOAT, FLOAT, FLOAT);
	void SetRotation(D3DXQUATERNION);
	void SetEuler(FLOAT, FLOAT, FLOAT);
	void SetEuler(D3DXVECTOR3);
	void SetScale(FLOAT, FLOAT, FLOAT);
	void SetScale(D3DXVECTOR3);

	void GetPosition(D3DXVECTOR3&);
	void GetRotation(D3DXQUATERNION&);
	void GetEuler(D3DXVECTOR3&);
	void GetScale(D3DXVECTOR3&);
	void GetUp(D3DXVECTOR3&);
	void GetForward(D3DXVECTOR3&);
	void GetRight(D3DXVECTOR3&);
	void GetLocalToWorld(D3DXMATRIX&);

	bool IsMatrixWillChange();

private:
	void RefreshMatrix();
	void RefreshEuler();
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_euler;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_forward;
	D3DXQUATERNION m_rotation;

	bool m_isMatrixChanged;
	bool m_isEulerChanged;

	D3DXMATRIX m_localToWorld;
};