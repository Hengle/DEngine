#include "DLight.h"


DLight::DLight()
{
}


DLight::~DLight()
{
}

void DLight::GetColor(DColor& color)
{
	color = m_color;
}

void DLight::SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

void DLight::SetColor(DColor color)
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
