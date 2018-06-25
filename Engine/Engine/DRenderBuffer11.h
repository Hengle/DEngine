#pragma once
#include "D3D11Core.h"

class DColorBuffer11 : public DRenderBuffer
{
public:
	DColorBuffer11();
	~DColorBuffer11();
	static DColorBuffer11 Create(ID3D11Texture2D*, const D3D11_RENDER_TARGET_VIEW_DESC);
};

class DDepthBuffer11 : public DRenderBuffer
{

};

