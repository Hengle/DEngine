#pragma once
#include "DMesh.h"
#include "DMaterial.h"

class DCameraFilter
{
public:
	DCameraFilter();
	~DCameraFilter();
	void Render(float, float);
	void Release();

protected:

private:
	void InitOutputPlane(float, float);

private:
	DMesh* m_mesh;
	DShader* m_shader;
	DTexture2D* m_tex;
	DMaterial* m_material;
};

