#pragma once
#include "DGLCore.h"
#include <map>
#include <vector>
#include <string>

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

class DShaderBuffer10 : public DShaderBuffer
{
private:
	class ShaderParam10
	{
	public:
		ShaderParam10()
		{
			bufferIndex = -1;
			bufferOffset = -1;
			bufferLength = 0;
			shaderType = 0;
			paramLength = 0;
			paramOffset = -1;
		}

		ShaderParam10(int bufferIndex, int bufferOffset, int bufferLength, int paramOffset, int paramLength, int shaderType)
		{
			this->bufferIndex = bufferIndex;
			this->bufferOffset = bufferOffset;
			this->bufferLength = bufferLength;
			this->shaderType = shaderType;
			this->paramOffset = paramOffset;
			this->paramLength = paramLength;
		}
	public:
		int bufferIndex, bufferOffset, bufferLength;
		int paramOffset, paramLength;
		int shaderType;
	};

public:
	DShaderBuffer10();
	~DShaderBuffer10();
	void Init(ID3D10Device* device, WCHAR*, WCHAR*);
	virtual unsigned int GetCBufferCount() const;
	virtual unsigned int GetPropertyCount() const;
	virtual void GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int& poffset, int& plength, int& stype) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();
	void ApplyBuffer(ID3D10Device *, int cindex, int coffset, int csize, int stype, float* params);
	void Draw(ID3D10Device*, int indexCount);

private:
	bool InitShader(ID3D10Device*, WCHAR*, WCHAR*);
	HRESULT InitVertexShader(ID3DBlob*, ID3D10Device*, ID3D10InputLayout**, int*);
	HRESULT InitPixelShader(ID3DBlob*, ID3D10Device*);

private:
	ID3D10VertexShader* m_vertexShader;
	ID3D10PixelShader *m_pixelShader;
	ID3D10InputLayout* m_layout;
	int m_cbufferCount;
	int m_propertyCount;
	std::map<const std::string, ShaderParam10> m_params;
	std::vector<ID3D10Buffer*> m_paramBuffers;
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

