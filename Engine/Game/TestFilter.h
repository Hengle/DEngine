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
	DRenderTexture* m_dst;
	//DShader* m_shader;
	//DShader* testshadow;
};

