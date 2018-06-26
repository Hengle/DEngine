#pragma once
#include "DCamera.h"
#include "DMaterial.h"

class TestFilter : public DCameraFilter
{
public:
	TestFilter();
	~TestFilter();

	virtual void Render(DRenderTexture* screenTexture);
	virtual void Release();

private:
	DMaterial* m_mat;
	DShader* m_shader;
};

