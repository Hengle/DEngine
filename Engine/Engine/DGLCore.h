#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h>

class DMeshBuffer
{
public:
	DMeshBuffer();
	~DMeshBuffer();
	virtual void Release() = 0;
};

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

class DTextureBuffer
{
public:
	DTextureBuffer();
	~DTextureBuffer();
	virtual void Release() = 0;
};

class DTextureBuffer11 : public DTextureBuffer
{
public:
	DTextureBuffer11(ID3D11ShaderResourceView*);
	virtual void Release();

private:
	ID3D11ShaderResourceView* m_resourceView;
};

class DGLCore
{
public:
	DGLCore();
	~DGLCore();
	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy() = 0;
	virtual void BeginRender(float, float, float, float) = 0;
	virtual void EndRender() = 0;
	virtual DMeshBuffer* CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices) = 0;
	virtual DTextureBuffer* CreateTextureBuffer(WCHAR* fileName) = 0;
	virtual void DrawMesh(const DMeshBuffer*, int) = 0;

	virtual void SetBackBufferRenderTarget() = 0;
	void GetResolution(FLOAT&, FLOAT&);

protected:

	FLOAT m_width;
	FLOAT m_height;
};

class D3DCore11 : public DGLCore
{
public:
	D3DCore11();
	~D3DCore11();
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

class D3DCore10 : public DGLCore
{
public:
	D3DCore10();

};