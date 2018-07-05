#pragma once
#include "DSceneObject.h"
#include "DColor.h"

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
	void SetIntensity(float);
	virtual void Update();

protected:
	DColor m_color;
	float m_intensity;
};

