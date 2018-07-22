#pragma once
#ifdef _DGAPI_D3D10

#include "D3D10Core.h"
#include <string>
#include <vector>
#include <map>

/*Shader程序Direct3d10实现*/
class DShaderProgram10 : public DShaderProgram
{
public:
	DShaderProgram10(ID3D10Device* device);
	~DShaderProgram10();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

protected:
	virtual void OnDraw();
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	virtual void OnSetValue(UINT, UINT, ID3D10Buffer* const *) = 0;

protected:
	ID3D10Device* m_device;
	ID3D10InputLayout* m_layout;
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderCBufferDesc*> m_cbuffers;
	std::vector<ID3D10Buffer*> m_paramBuffers;
};

/*顶点着色器direct3d10实现*/
class DShaderVertexProgram10 : public DShaderProgram10
{
public:
	DShaderVertexProgram10(ID3D10Device* device);
	~DShaderVertexProgram10();
	virtual void Release();

private:
	HRESULT InitVertexShader(ID3DBlob*, ID3D10Device*, ID3D10InputLayout**, int*);

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();
	virtual void OnSetValue(UINT, UINT, ID3D10Buffer* const *);

private:
	ID3D10VertexShader* m_vertexShader;
};

/*像素着色器direct3d10实现*/
class DShaderPixelProgram10 : public DShaderProgram10
{
public:
	DShaderPixelProgram10(ID3D10Device* device);
	~DShaderPixelProgram10();
	virtual void Release();

private:
	HRESULT InitPixelShader(ID3DBlob*, ID3D10Device*);

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();
	virtual void OnSetValue(UINT, UINT, ID3D10Buffer* const *);

private:
	ID3D10PixelShader *m_pixelShader;
};

#endif