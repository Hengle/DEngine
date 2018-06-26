#pragma once
#include "DSceneObject.h"
#include "DCameraFilter.h"

class DCamera : public DSceneObject
{
public:
	DCamera();
	~DCamera();
	void BeginRender();
	void EndRender();
	void RenderFilter();
	virtual void Init();
	virtual void Destroy();
	void GetViewMatrix(DMatrix4x4&) const;
	void GetProjection(DMatrix4x4&) const;
	float GetFieldOfView() const;
	float GetNear() const;
	float GetFar() const;
	float GetAspect() const;
	float GetOrthoSize() const;
	bool IsOrthographic() const;
	void SetFieldOfView(float);
	void SetNear(float);
	void SetFar(float);
	void SetAspect(float);
	void SetOrthographic(bool);
	void SetOrthoSize(float);
	void SetFilter(DCameraFilter*);
	void ClearFilter();
	DRenderTexture* GetRenderTexture();
	void SetRenderTexture(DRenderTexture*);

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
	DCameraFilter* m_filter;
	DRenderTexture* m_renderTexture;
};

static DCamera* sCurrent;

