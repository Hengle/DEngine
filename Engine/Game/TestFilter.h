#pragma once
#include "DCamera.h"
#include "DMaterial.h"

class TestFilter : public ICameraFilter
{
public:
	TestFilter();
	~TestFilter();

	virtual DRenderTexture* Render(DRenderTexture* screenTexture);
	virtual void Release();

public:
	float nearV;
	float farV;

private:
	DMaterial* m_mat;
	//DMaterial* m_glowMat;
	DMaterial* m_mixMat;
	DRenderTexture* m_dst;
	DRenderTexture* m_dst2;
	//DShader* m_shader;
	//DShader* testshadow;
};

