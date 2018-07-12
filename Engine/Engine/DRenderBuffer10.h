#pragma once
#include "D3D10Core.h"

class DColorBuffer10 : public IRenderBuffer
{
public:
	DColorBuffer10();
	~DColorBuffer10();
	virtual void Release();
	static DColorBuffer10* Create(ID3D10Device*, ID3D10Resource*, const D3D10_RENDER_TARGET_VIEW_DESC*);
	ID3D10RenderTargetView* GetView();

private:
	ID3D10RenderTargetView* m_renderTargetView;
};

class DDepthBuffer10 : public IRenderBuffer
{
public:
	DDepthBuffer10();
	~DDepthBuffer10();
	virtual void Release();
	static DDepthBuffer10* Create(ID3D10Device*, ID3D10Resource*, const D3D10_DEPTH_STENCIL_VIEW_DESC*);
	ID3D10DepthStencilView* GetView();

private:
	ID3D10DepthStencilView* m_depthStencilView;
};

