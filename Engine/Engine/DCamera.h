#pragma once
#include "DSceneObject.h"

class DCamera : public DSceneObject
{
public:
	DCamera();
	~DCamera();
	void BeginRender();
	void EndRender();
	virtual void Init();
	virtual void Destroy();
	void GetViewMatrix(DMatrix4x4&);
	void GetProjection(DMatrix4x4&);
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
	float m_fieldOfView;
	float m_near;
	float m_far;
	float m_aspect;
	float m_orthoSize;
	bool m_ortho;
	DMatrix4x4 m_viewMatrix;
	DMatrix4x4 m_projection;
	bool m_isProjectionChanged;

};

static DCamera* sCurrent;

