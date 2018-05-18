#pragma once
#include "DResObject.h"
#include <d3d11.h>
#include <d3dx11async.h>
#include <d3dx10math.h>
#include <fstream>

class DShader : public DResObject
{
protected:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	DShader(WCHAR*, WCHAR*);
	~DShader();

	virtual void Init(ID3D11Device*);
	virtual void Destroy();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

protected:
	virtual bool InitializeShader(ID3D11Device*, WCHAR*, WCHAR*);
	virtual void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	virtual void RenderShader(ID3D11DeviceContext*, int);

protected:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	WCHAR* m_vertexShaderFile;
	WCHAR* m_pixelShaderFile;
};

class DTexShader : public DShader
{
public:
	DTexShader(WCHAR*, WCHAR*);
	~DTexShader();

	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

protected:
	virtual bool InitializeShader(ID3D11Device*, WCHAR*, WCHAR*);
	virtual void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	virtual void RenderShader(ID3D11DeviceContext*, int);

protected:
	ID3D11SamplerState* m_samplerState;
};

class DLightShader : public DTexShader
{
private:
	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};
public:
	DLightShader(WCHAR*, WCHAR*);
	~DLightShader();

	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);

private:
	virtual bool InitializeShader(ID3D11Device*, WCHAR*, WCHAR*);
	virtual void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);

private:
	ID3D11Buffer* m_lightBuffer;
};