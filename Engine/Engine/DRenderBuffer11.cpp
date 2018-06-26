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

ID3D11RenderTargetView * DColorBuffer11::GetView()
{
	return m_renderTargetView;
}

DDepthBuffer11::DDepthBuffer11()
{
}

DDepthBuffer11::~DDepthBuffer11()
{
}

void DDepthBuffer11::Release()
{
	if (m_depthStencilView != NULL)
	{
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}
}

DDepthBuffer11* DDepthBuffer11::Create(ID3D11Device* device, ID3D11Resource * resource, const D3D11_DEPTH_STENCIL_VIEW_DESC* desc)
{
	ID3D11DepthStencilView* view;
	HRESULT result = device->CreateDepthStencilView(resource, desc, &view);
	if (!FAILED(result))
	{
		DDepthBuffer11* buffer = new DDepthBuffer11();
		buffer->m_depthStencilView = view;
		return buffer;
	}
	return NULL;
}

ID3D11DepthStencilView * DDepthBuffer11::GetView()
{
	return m_depthStencilView;
}
