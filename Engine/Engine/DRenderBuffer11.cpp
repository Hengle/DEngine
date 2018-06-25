#include "DRenderBuffer11.h"


DColorBuffer11::DColorBuffer11()
{
}


DColorBuffer11::DColorBuffer11(ID3D11Texture2D *, const D3D11_RENDER_TARGET_VIEW_DESC)
{
}

DColorBuffer11::~DColorBuffer11()
{
}

DColorBuffer11 DColorBuffer11::Create(ID3D11Texture2D *, const D3D11_RENDER_TARGET_VIEW_DESC)
{
	return DColorBuffer11();
}
