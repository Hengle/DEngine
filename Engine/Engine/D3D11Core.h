#pragma once
#include "DGLCore.h"

class DMeshBuffer11 : public DMeshBuffer
{
public:
	DMeshBuffer11(ID3D11Buffer*, ID3D11Buffer*);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	virtual void Release();

private:
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
};

class DTextureBuffer11 : public DTextureBuffer
{
public:
	DTextureBuffer11(ID3D11ShaderResourceView*);
	virtual void Release();

private:
	ID3D11ShaderResourceView* m_resourceView;
};

class D3D11Core : public DGLCore
{
public:
	D3D11Core();
	~D3D11Core();
	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender(float, float, float, float);
	virtual void EndRender();
	virtual DMeshBuffer* CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices);
	virtual DTextureBuffer* CreateTextureBuffer(WCHAR* fileName);
	virtual void DrawMesh(const DMeshBuffer*, int);

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
	D3D11_VIEWPORT m_viewPort;
};

