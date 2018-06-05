#pragma once
#include "DGLCore.h"

class DMeshBuffer10 : public DMeshBuffer
{
public:
	DMeshBuffer10();
	void Init(ID3D10Device*, int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices);
	ID3D10Buffer* GetVertexBuffer();
	ID3D10Buffer* GetIndexBuffer();
	virtual void Release();

private:
	ID3D10Buffer* m_vertexBuffer, *m_indexBuffer;
};

class D3D10Core : public DGLCore
{
public:
	D3D10Core();
	~D3D10Core();

	virtual bool Init(int, int, bool, HWND);
	virtual void Destroy();
	virtual void BeginRender(float, float, float, float);
	virtual void EndRender();
	virtual DMeshBuffer* CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices);
	virtual DTextureBuffer* CreateTextureBuffer(WCHAR* fileName);
	virtual DShaderBuffer* CreateShaderBuffer(WCHAR* vertexShader, WCHAR* pixelShader);
	virtual void ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float* params);
	virtual void ApplyTextureParams(DTextureBuffer* textureBuffer);
	virtual void DrawMesh(const DMeshBuffer*, int);
	virtual void DrawShader(const DShaderBuffer*, int);

	virtual void SetBackBufferRenderTarget();

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

