#include "DLight.h"


DLight::DLight()
{
}


DLight::~DLight()
{
}

void DLight::GetColor(D3DXVECTOR4& color)
{
	color = m_color;
}

void DLight::SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
	m_color.w = a;
}

void DLight::SetColor(D3DXVECTOR4 color)
{
	m_color = color;
}

float DLight::GetIntensity()
{
	return m_intensity;
}

void DLight::SetIntensity(float intensity)
{
	m_intensity = intensity;
}
