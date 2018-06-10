#pragma once
#include "DGLCore.h"
#include <dxgi.h>
#include <D3D11.h>
#include <D3DX11.h>

class D3D11Core : public DGLCore
{
public:
	D3D11Core();
	~D3D11Core();
	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender();
	virtual void EndRender();
	virtual DMeshRes* CreateMeshRes();
	virtual DTextureRes* CreateTextureRes();
	virtual DShaderRes* CreateShaderRes();

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView*m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	D3D11_VIEWPORT m_viewPort;
};
