#pragma once
#include "DSceneObject.h"
#include "DColor.h"
#include "DTexture.h"
#include "DShader.h"

enum DLightType
{
	Directional,
	Point,
};

class DLight : public DSceneObject
{
public:
	DLight();
	~DLight();
	void GetColor(DColor&);
	void SetColor(float, float, float, float);
	void SetColor(DColor);
	float GetIntensity();
	float GetFar();
	float GetSize();
	void SetFar(float);
	void SetSize(float);
	void SetIntensity(float);
	void RenderShadow();
	virtual void Init();
	virtual void Destroy();
	virtual void Update();

private:
	void BeginRenderShadow();
	void EndRenderShadow();
	//void GetCameraBounds(DCamera* cam, DVector3* outCenter, DVector3* outSize);

private:
	DColor m_color;
	float m_intensity;
	DRenderTexture* m_shadowMap;
	DShader* m_shadowShader;
	DMatrix4x4 m_view;
	DMatrix4x4 m_proj;
	float m_far;
	float m_size;
	bool m_isProjChanged;
};

