#pragma once
#include "DSceneObject.h"

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
	void GetColor(D3DXVECTOR4&);
	void SetColor(FLOAT, FLOAT, FLOAT, FLOAT);
	void SetColor(D3DXVECTOR4);
	float GetIntensity();
	void SetIntensity(float);

protected:
	D3DXVECTOR4 m_color;
	float m_intensity;
};

