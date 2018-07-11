#pragma once
#include "D3D9Core.h"
#include <string>
#include <vector>
#include <map>

class DShaderProgram9 : public DShaderProgram
{
protected:
	/*描述shader属性*/
	struct DShaderPropertyDesc9 : public DShaderPropertyDesc
	{
	public:
		DShaderPropertyDesc9() :DShaderPropertyDesc()
		{
			isGlobal = false;
			shaderType = 0;
		}
	public:
		bool isGlobal;
		int shaderType;
	};

public:
	DShaderProgram9(LPDIRECT3DDEVICE9 device);
	~DShaderProgram9();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

protected:
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	virtual void OnSetValue(D3DXHANDLE&, float*, UINT) = 0;

protected:
	LPDIRECT3DDEVICE9 m_device;
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderPropertyDesc9> m_properties;
	std::vector<D3DXHANDLE> m_handles;
};

class DShaderVertexProgram9 : public DShaderProgram9
{
public:
	DShaderVertexProgram9(LPDIRECT3DDEVICE9 device);
	~DShaderVertexProgram9();
	virtual void Release();

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();
	virtual void OnSetValue(D3DXHANDLE&, float*, UINT);

private:
	HRESULT InitVertexShader(ID3DXBuffer*);

private:
	IDirect3DVertexShader9* m_vertexShader;
	ID3DXConstantTable* m_vertexConstable;
};

class DShaderPixelProgram9 : public DShaderProgram9
{
public:
	DShaderPixelProgram9(LPDIRECT3DDEVICE9 device);
	~DShaderPixelProgram9();
	virtual void Release();

protected:
	virtual bool OnInit(const char* content, char* funcName);
	virtual void OnDraw();
	virtual void OnSetValue(D3DXHANDLE&, float*, UINT);

private:
	HRESULT InitPixelShader();

private:
	IDirect3DPixelShader9* m_pixelShader;
	ID3DXConstantTable* m_pixelConstable;
};

