#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h>

class D3DCore
{
public:
	D3DCore();
	~D3DCore();
	bool Init(int, int, bool, HWND);
	void Destroy();
	void BeginRender(float, float, float, float);
	void EndRender();
	ID3D11Device*GetDevice();
	ID3D11DeviceContext*GetDeviceContext();
	void GetResolution(FLOAT&, FLOAT&);


private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView*m_renderTargetView;
	FLOAT m_width;
	FLOAT m_height;
};

