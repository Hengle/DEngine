#include "stdafx.h"
#include "WaterFilter.h"
#include "DGraphics.h"

WaterFilter::WaterFilter()
{
}


WaterFilter::~WaterFilter()
{
}

DRenderTexture * WaterFilter::Render(DCamera *, DRenderTexture * screenTexture)
{
	if (m_grab == NULL)
	{
		float w, h;
		w = screenTexture->GetWidth();
		h = screenTexture->GetHeight();
		m_grab = DRenderTexture::Create(w, h);
	}
	DGraphics::Blit(screenTexture, m_grab);
	DShader::SetGlobalTexture("g_grabTexture", screenTexture);
	return screenTexture;
}

void WaterFilter::Release()
{
	if (m_grab != NULL)
	{
		m_grab->Destroy();
		delete m_grab;
		m_grab = 0;
	}
}
