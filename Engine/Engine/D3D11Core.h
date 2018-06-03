#pragma once
#include "DGLCore.h"
#include <map>
#include <vector>
#include <string>
#include "DMaterial.h"

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

//class DShaderParam11 :public DShaderParam
//{
//public:
//	DShaderParam11(int count);
//	void SetParam(ID3D11Device* device, int index, int size);
//	virtual void BeginSetParam(int id, void** value);
//	virtual void EndSetParam(int id);
//	virtual void Release();
//
//private:
//	ID3D11Buffer** m_paramsBuffer;
//	int m_count;
//};

class DShaderBuffer11 :public DShaderBuffer
{
private:
	struct ShaderProperty
	{
	public:
		ShaderProperty()
		{
			index = -1;
			length = 0;
			offset = 0;
		}

		ShaderProperty(int index, int length, int offset)
		{
			this->index = index;
			this->length = length;
			this->offset = offset;
		}

	public:
		int index, length, offset;
	};

	class ShaderParam
	{
	public:
		ShaderParam()
		{
			bufferIndex = -1;
			bufferOffset = -1;
			bufferLength = 0;
			shaderType = 0;
			propertyCount = 0;
		}
		
		ShaderParam(int bufferIndex, int bufferOffset, int bufferLength, int shaderType, int propertyCount)
		{
			this->bufferIndex = bufferIndex;
			this->bufferOffset = bufferOffset;
			this->bufferLength = bufferLength;
			this->shaderType = shaderType;
			this->propertyCount = propertyCount;
		}
	public:
		int bufferIndex, bufferOffset, bufferLength;
		int shaderType;
		int propertyCount;
		std::map<const std::string, ShaderProperty> properties;
	};

public:
	DShaderBuffer11();
	~DShaderBuffer11();
	void Init(ID3D11Device* device, WCHAR*, WCHAR*);
	virtual unsigned int GetCBufferCount() const;
	virtual unsigned int GetPropertyCount(LPCSTR cbuffername) const;
	virtual int GetCBufferIndex(LPCSTR cbuffername) const;
	virtual int GetCBufferOffset(LPCSTR cbuffername) const;
	virtual int GetCBufferLength(LPCSTR cbuffername) const;
	virtual int GetCBufferType(LPCSTR cbuffername) const;
	virtual int GetPropertyIndex(const LPCSTR cbufferName, const LPCSTR key) const;
	virtual int GetPropertyOffset(const LPCSTR cbufferName, const LPCSTR key) const;
	virtual int GetPropertyLength(const LPCSTR cbufferName, const LPCSTR key) const;
	virtual void GetPropertyInfo(const LPCSTR cbufferName, const LPCSTR key, int&, int&, int&) const;
	virtual void GetCBufferInfo(LPCSTR, int&, int&, int&, int&) const;
	//virtual DShaderParam* GetParams() const;
	virtual void Release();
	void ApplyBuffer(ID3D11DeviceContext * deviceContext, int cindex, int coffset, int csize, int stype, float* params);
	void Draw(ID3D11DeviceContext*, int indexCount);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*);
	HRESULT InitVertexShader(ID3DBlob*, ID3D11Device*, ID3D11InputLayout**, int*);
	HRESULT InitPixelShader(ID3DBlob*, ID3D11Device*);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader *m_pixelShader;
	ID3D11InputLayout* m_layout;
	int m_cbufferCount;
	std::map<const std::string, ShaderParam> m_params;
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
	virtual void ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float* params);
	virtual void DrawMesh(const DMeshBuffer*, int);
	virtual void DrawShader(const DShaderBuffer*, int);

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
