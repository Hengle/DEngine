#pragma once
#include "DCamera.h"
#include "DMaterial.h"

class TestFilter : public ICameraFilter
{
public:
	TestFilter();
	~TestFilter();

	virtual void Render(DRenderTexture* screenTexture);
	virtual void Release();

private:
	DMaterial* m_mat;
	DMaterial* m_glowMat;
	DMaterial* m_mixMat;
	DRenderTexture* m_dst;
	DRenderTexture* m_dst2;
	//DShader* m_shader;
	//DShader* testshadow;
};

