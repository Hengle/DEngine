#pragma once
#include "d3d11.h"
#include "DSceneObject.h"

class DCamera : public DSceneObject
{
public:
	DCamera();
	~DCamera();
	void BeginRender(ID3D11DeviceContext*);
	void EndRender();
	virtual void Init();
	virtual void Destroy();
	void GetViewMatrix(D3DXMATRIX&);
	void GetProjection(D3DXMATRIX&);
	float GetFieldOfView();
	float GetNear();
	float GetFar();
	float GetAspect();
	float GetOrthoSize();
	bool IsOrthographic();
	void SetFieldOfView(float);
	void SetNear(float);
	void SetFar(float);
	void SetAspect(float);
	void SetOrthographic(bool);
	void SetOrthoSize(float);
	
	static void GetCurrentCamera(DCamera** cam);

private:
	D3D11_VIEWPORT m_viewPort;
	float m_fieldOfView;
	float m_near;
	float m_far;
	float m_aspect;
	float m_orthoSize;
	bool m_ortho;
	//D3DXVECTOR3 m_position;
	//D3DXQUATERNION m_rotation;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projection;
	bool m_isProjectionChanged;
	
};

static DCamera* sCurrent;
