#ifdef _DGAPI_D3D10
#include "DRenderBuffer10.h"

DColorBuffer10::DColorBuffer10()
{
}

DColorBuffer10::~DColorBuffer10()
{
}

void DColorBuffer10::Release()
{
	if (m_renderTargetView != NULL)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULL;
	}
}

DColorBuffer10 * DColorBuffer10::Create(ID3D10Device * device, ID3D10Resource *resource, const D3D10_RENDER_TARGET_VIEW_DESC *desc)
{
	ID3D10RenderTargetView* view;
	HRESULT result = device->CreateRenderTargetView(resource, desc, &view);
	if (!FAILED(result))
	{
		DColorBuffer10* buffer = new DColorBuffer10();
		buffer->m_renderTargetView = view;
		return buffer;
	}
	return NULL;
}

ID3D10RenderTargetView * DColorBuffer10::GetView()
{
	return m_renderTargetView;
}

DDepthBuffer10::DDepthBuffer10()
{
}

DDepthBuffer10::~DDepthBuffer10()
{
}

void DDepthBuffer10::Release()
{
	if (m_depthStencilView != NULL)
	{
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}
}

DDepthBuffer10 * DDepthBuffer10::Create(ID3D10Device * device, ID3D10Resource *resource, const D3D10_DEPTH_STENCIL_VIEW_DESC *desc)
{
	ID3D10DepthStencilView* view;
	HRESULT result = device->CreateDepthStencilView(resource, desc, &view);
	if (!FAILED(result))
	{
		DDepthBuffer10* buffer = new DDepthBuffer10();
		buffer->m_depthStencilView = view;
		return buffer;
	}
	return NULL;
}

ID3D10DepthStencilView * DDepthBuffer10::GetView()
{
	return m_depthStencilView;
}
#endif