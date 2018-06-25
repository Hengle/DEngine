#include "DRenderBuffer11.h"


DColorBuffer11::DColorBuffer11()
{
}

DColorBuffer11::~DColorBuffer11()
{
}

void DColorBuffer11::Release()
{
	if (m_renderTargetView != NULL)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULL;
	}
}

DColorBuffer11* DColorBuffer11::Create(ID3D11Device* device, ID3D11Resource * resource, const D3D11_RENDER_TARGET_VIEW_DESC* desc)
{
	ID3D11RenderTargetView* view;
	HRESULT result = device->CreateRenderTargetView(resource, desc, &view);
	if (!FAILED(result))
	{
		DColorBuffer11* buffer = new DColorBuffer11();
		buffer->m_renderTargetView = view;
		return buffer;
	}
	return NULL;
}

DDepthBuffer11::DDepthBuffer11()
{
}

DDepthBuffer11::~DDepthBuffer11()
{
}

void DDepthBuffer11::Release()
{
}

DDepthBuffer11* DDepthBuffer11::Create(ID3D11Device*, ID3D11Resource *, const D3D11_DEPTH_STENCIL_VIEW_DESC* desc)
{
	return DDepthBuffer11();
}
