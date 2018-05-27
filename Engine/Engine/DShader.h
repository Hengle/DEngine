#pragma once
#include "DResObject.h"
#include "DMath.h"
#include "DColor.h"
#include <fstream>
#include <D3DX11.h>
#include <d3dcompiler.h>

class DShader : public DResObject
{
protected:
	struct MatrixBufferType
	{
		DMatrix4x4 world;
		DMatrix4x4 view;
		DMatrix4x4 projection;
	};
public:
	DShader(WCHAR*, WCHAR*);
	~DShader();

	virtual void Init(ID3D11Device*);
	virtual void Destroy();
	bool Render(ID3D11DeviceContext*, int, DMatrix4x4, DMatrix4x4, DMatrix4x4);

protected:
	virtual bool InitializeShader(ID3D11Device*, WCHAR*, WCHAR*);
	virtual void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DMatrix4x4, DMatrix4x4, DMatrix4x4);
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

	bool Render(ID3D11DeviceContext*, int, DMatrix4x4, DMatrix4x4, DMatrix4x4, ID3D11ShaderResourceView*);

protected:
	virtual bool InitializeShader(ID3D11Device*, WCHAR*, WCHAR*);
	virtual void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, DMatrix4x4, DMatrix4x4, DMatrix4x4, ID3D11ShaderResourceView*);
	virtual void RenderShader(ID3D11DeviceContext*, int);

protected:
	ID3D11SamplerState* m_samplerState;
};

class DLightShader : public DTexShader
{
private:
	struct LightBufferType
	{
		DColor diffuseColor;
		DVector3 lightDirection;
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};
public:
	DLightShader(WCHAR*, WCHAR*);
	~DLightShader();

	bool Render(ID3D11DeviceContext*, int, DMatrix4x4, DMatrix4x4, DMatrix4x4, ID3D11ShaderResourceView*, DVector3, DColor);

private:
	virtual bool InitializeShader(ID3D11Device*, WCHAR*, WCHAR*);
	virtual void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, DMatrix4x4, DMatrix4x4, DMatrix4x4, ID3D11ShaderResourceView*, DVector3, DColor);

private:
	ID3D11Buffer* m_lightBuffer;
};