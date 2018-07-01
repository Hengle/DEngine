#pragma once
#include "D3D9Core.h"
#include <string>
#include <vector>
#include <map>

class DShaderRes9 : public DShaderRes
{
private:
	struct DShaderPropertyDesc9 : public DShaderPropertyDesc
	{
	public:
		DShaderPropertyDesc9():DShaderPropertyDesc()
		{
			isGlobal = false;
			shaderType = 0;
		}
	public:
		bool isGlobal;
		int shaderType;
	};

public:
	DShaderRes9(LPDIRECT3DDEVICE9 device);
	~DShaderRes9();
	//virtual void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const;
	//virtual UINT GetResOffset(const LPCSTR key) const;
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

protected:
	virtual bool OnInit(WCHAR*, WCHAR*);
	virtual void OnDraw();
	//virtual void OnApplyParams(int, int, int, int, float*);
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);

private:
	HRESULT InitVertexShader(ID3DXBuffer*);
	HRESULT InitPixelShader();

private:
	LPDIRECT3DDEVICE9 m_device;
	IDirect3DVertexShader9* m_vertexShader;
	IDirect3DPixelShader9* m_pixelShader;
	ID3DXConstantTable* m_vertexConstable;
	ID3DXConstantTable* m_pixelConstable;
	//std::map<std::string, DShaderParamDesc> m_params;
	//std::map<std::string, D3DXCONSTANT_DESC> m_resParams;
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderPropertyDesc9> m_properties;
	std::vector<D3DXHANDLE> m_handles;
};

