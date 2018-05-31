#pragma once
#include "DGLCore.h"
#include <map>
#include <vector>

class DMeshBuffer11 : public DMeshBuffer
{
public:
	DMeshBuffer11();
	void Init(ID3D11Device*, int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	virtual void Release();

private:
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
};

class DTextureBuffer11 : public DTextureBuffer
{
public:
	DTextureBuffer11();
	void Init(ID3D11Device* device, WCHAR* fileName);
	virtual void Release();

private:
	ID3D11ShaderResourceView* m_resourceView;
};

class DShaderBuffer11 :public DShaderBuffer
{
public:
	DShaderBuffer11();
	~DShaderBuffer11();
	void Init(ID3D11Device* device, WCHAR*, WCHAR*);
	virtual unsigned int GetCBufferCount() const;
	virtual int GetCBufferIndex(LPCSTR cbuffername) const;
	virtual void ApplyParam(void*);
	virtual void Release();

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*);
	HRESULT InitShaderParams(ID3DBlob*, ID3D11Device*, ID3D11InputLayout**, int*);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader *m_pixelShader;
	ID3D11InputLayout* m_layout;
	int m_cbufferCount;
	std::map<LPCSTR, int> m_paramIds;
	std::vector<ID3D11Buffer*> m_paramBuffers;
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
	virtual DShaderBuffer* CreateShaderBuffer(WCHAR* vertexShader, WCHAR* pixelShader);
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

