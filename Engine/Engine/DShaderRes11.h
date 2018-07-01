﻿#pragma once
#include "D3D11Core.h"
//#include <map>
#include <vector>
//#include <string>

class DShaderRes11 : public DShaderRes
{
public:
	DShaderRes11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DShaderRes11();
	//virtual void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const;
	//virtual UINT GetResOffset(const LPCSTR key) const;
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

private:
	HRESULT InitVertexShader(ID3DBlob*, ID3D11Device*, ID3D11InputLayout**, int*);
	HRESULT InitPixelShader(ID3DBlob*, ID3D11Device*);

protected:
	virtual bool OnInit(WCHAR*, WCHAR*);
	virtual void OnDraw();
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	//virtual void OnApplyParams(int, int, int, int, float*);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader *m_pixelShader;
	ID3D11InputLayout* m_layout;
	//std::map<const std::string, DShaderCBufferDesc> m_cbuffers;
	//std::map<const std::string, DShaderResDesc> m_resParams;
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderCBufferDesc*> m_cbuffers;
	std::vector<ID3D11Buffer*> m_paramBuffers;
};

