#pragma once
#include "DSceneObject.h"
#include "DColor.h"
#include "DTexture.h"
#include "DShader.h"

enum DLightType
{
	DLightType_Directional,
	DLightType_Point,
};

class DLight;

struct DLightNode
{
public:
	DLight* light;
	DLightNode* next;
	DLightNode* pre;
};

class DLight : public DSceneObject
{
public:
	DLight();
	~DLight();
	void RenderShadow();
	void GetColor(DColor&);
	void SetColor(float, float, float, float);
	void SetColor(DColor);
	float GetIntensity();
	float GetFar();
	float GetSize();
	void SetFar(float);
	void SetSize(float);
	void SetIntensity(float);

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();

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

	DLightNode* m_node;
};

