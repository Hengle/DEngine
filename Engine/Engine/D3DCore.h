#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h>

class D3DCoreBase
{
public:
	D3DCoreBase();
	~D3DCoreBase();
	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy() = 0;
	virtual void BeginRender(float, float, float, float) = 0;
	virtual void EndRender() = 0;
	
	virtual void SetBackBufferRenderTarget() = 0;
	void GetResolution(FLOAT&, FLOAT&);

protected:
	
	FLOAT m_width;
	FLOAT m_height;
};

class D3DCore11 : public D3DCoreBase
{
public:
	D3DCore11();
	~D3DCore11();
	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender(float, float, float, float);
	virtual void EndRender();

	virtual void SetBackBufferRenderTarget();

	ID3D11Device*GetDevice();
	ID3D11DeviceContext*GetDeviceContext();
	ID3D11DepthStencilView* GetDepthStencilView();

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView*m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
};

class D3DCore10 : public D3DCoreBase
{
public:
	D3DCore10();

};

