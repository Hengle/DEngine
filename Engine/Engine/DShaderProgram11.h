#pragma once
#include "D3D11Core.h"
#include <vector>

class DShaderProgram11 : public DShaderProgram
{
public:
	DShaderProgram11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DShaderProgram11();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

protected:
	virtual void OnDraw();
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	void OutputShaderErrorMessage(ID3D10Blob*);

protected:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11InputLayout* m_layout;
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderCBufferDesc*> m_cbuffers;
	std::vector<ID3D11Buffer*> m_paramBuffers;
};

class DShaderVertexProgram11 : public DShaderProgram11
{
public:
	DShaderVertexProgram11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DShaderVertexProgram11();
	virtual void Release();

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();

private:
	HRESULT InitVertexShader(ID3DBlob*, ID3D11Device*, ID3D11InputLayout**, int*);

private:
	ID3D11VertexShader* m_vertexShader;
};

class DShaderPixelProgram11 : public DShaderProgram11
{
public:
	DShaderPixelProgram11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DShaderPixelProgram11();
	virtual void Release();

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();

private:
	HRESULT InitPixelShader(ID3DBlob*, ID3D11Device*);

private:
	ID3D11PixelShader *m_pixelShader;
};
