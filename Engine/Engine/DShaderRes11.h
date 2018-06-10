#pragma once
#include "D3D11Core.h"

class DShaderRes11 : public DShaderRes
{
public:
	DShaderRes11(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~DShaderRes11();
	virtual void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

private:
	HRESULT InitVertexShader(ID3DBlob*, ID3D11Device*, ID3D11InputLayout**, int*);
	HRESULT InitPixelShader(ID3DBlob*, ID3D11Device*);

protected:
	virtual bool OnInit(WCHAR*, WCHAR*);
	virtual void OnDraw(int);
	virtual void OnApplyParams(int, int, int, int, float*);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader *m_pixelShader;
	ID3D11InputLayout* m_layout;
	std::map<const std::string, DShaderParamDesc> m_params;
	std::vector<ID3D11Buffer*> m_paramBuffers;
};

