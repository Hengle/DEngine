#pragma once
#include "DGLCore.h"
#include <d3d10.h>

class D3D10Core : public DGLCore
{
public:
	D3D10Core();
	~D3D10Core();

	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender();
	virtual void EndRender();
	virtual DMeshRes* CreateMeshRes();
	virtual DTextureRes* CreateTextureRes(WCHAR*);
	virtual DShaderRes* CreateShaderRes();
	virtual void ApplySamplerState(UINT, DWarpMode);
	ID3D10Device* GetDevice() const;

private:
	IDXGISwapChain* m_swapChain;
	ID3D10Device* m_device;
	ID3D10RenderTargetView*m_renderTargetView;
	ID3D10Texture2D* m_depthStencilBuffer;
	ID3D10DepthStencilState* m_depthStencilState;
	ID3D10DepthStencilView* m_depthStencilView;
	ID3D10RasterizerState* m_rasterState;
	D3D10_VIEWPORT m_viewPort;
};

