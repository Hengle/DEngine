#pragma once
#include "D3D11Core.h"
#include <vector>

/*Shader程序Direct3d11实现*/
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
	virtual void OnSetValue(UINT, UINT, ID3D11Buffer* const *) = 0;
	void OutputShaderErrorMessage(ID3D10Blob*);

protected:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11InputLayout* m_layout;
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderCBufferDesc*> m_cbuffers;
	std::vector<ID3D11Buffer*> m_paramBuffers;
};

/*顶点着色器direct3d11实现*/
class DShaderVertexProgram11 : public DShaderProgram11
{
public:
	DShaderVertexProgram11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DShaderVertexProgram11();
	virtual void Release();

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();
	virtual void OnSetValue(UINT, UINT, ID3D11Buffer* const *);

private:
	HRESULT InitVertexShader(ID3DBlob*, ID3D11Device*, ID3D11InputLayout**, int*);

private:
	ID3D11VertexShader* m_vertexShader;
};

/*像素着色器direct3d11实现*/
class DShaderPixelProgram11 : public DShaderProgram11
{
public:
	DShaderPixelProgram11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DShaderPixelProgram11();
	virtual void Release();

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();
	virtual void OnSetValue(UINT, UINT, ID3D11Buffer* const *);

private:
	HRESULT InitPixelShader(ID3DBlob*, ID3D11Device*);

private:
	ID3D11PixelShader *m_pixelShader;
};
