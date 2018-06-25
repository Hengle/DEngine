#pragma once
#include "D3D11Core.h"

class DRenderTarget11
{
public:
	DRenderTarget11();
	~DRenderTarget11();
	static DRenderTarget11* Create(ID3D11Texture2D*, const D3D11_RENDER_TARGET_VIEW_DESC*);
};

