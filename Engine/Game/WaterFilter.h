#pragma once
#include "DCamera.h"
#include "DMaterial.h"

class WaterFilter : public ICameraFilter
{
public:
	WaterFilter();
	~WaterFilter();

	virtual DRenderTexture* Render(DCamera*, DRenderTexture* screenTexture);
	virtual void Release();

private:
	DRenderTexture* m_grab;
};

