#pragma once
#include "D3D11Core.h"

class DColorBuffer11 : public IRenderBuffer
{
public:
	DColorBuffer11();
	~DColorBuffer11();
	virtual void Release();
	static DColorBuffer11* Create(ID3D11Device*, ID3D11Resource*, const D3D11_RENDER_TARGET_VIEW_DESC*);
	ID3D11RenderTargetView* GetView();

private:
	ID3D11RenderTargetView* m_renderTargetView;
};

class DDepthBuffer11 : public IRenderBuffer
{
public:
	DDepthBuffer11();
	~DDepthBuffer11();
	virtual void Release();
	static DDepthBuffer11* Create(ID3D11Device*, ID3D11Resource*, const D3D11_DEPTH_STENCIL_VIEW_DESC*);
	ID3D11DepthStencilView* GetView();

private:
	ID3D11DepthStencilView* m_depthStencilView;
};

