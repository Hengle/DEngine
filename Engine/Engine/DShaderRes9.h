#pragma once
#include "D3D9Core.h"
#include <string>
#include <vector>
#include <map>

/*
	Shader资源Direct3D9底层
*/
class DShaderRes9 : public DShaderRes
{
private:
	/*描述shader属性*/
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
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

protected:
	virtual bool OnInit(const char* content, char* vsfunc, char* psfunc);
	virtual void OnDraw();
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
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderPropertyDesc9> m_properties;
	std::vector<D3DXHANDLE> m_handles;
};

