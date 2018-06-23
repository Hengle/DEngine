#pragma once
#include "D3D10Core.h"
#include <string>
#include <vector>
#include <map>

class DShaderRes10 : public DShaderRes
{
public:
	DShaderRes10(ID3D10Device* device);
	~DShaderRes10();
	virtual void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const;
	virtual UINT GetResOffset(const LPCSTR key) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

private:
	HRESULT InitVertexShader(ID3DBlob*, ID3D10Device*, ID3D10InputLayout**, int*);
	HRESULT InitPixelShader(ID3DBlob*, ID3D10Device*);

protected:
	virtual bool OnInit(WCHAR*, WCHAR*);
	virtual void OnDraw();
	virtual void OnApplyParams(int, int, int, int, float*);

private:
	ID3D10Device* m_device;
	ID3D10VertexShader* m_vertexShader;
	ID3D10PixelShader *m_pixelShader;
	ID3D10InputLayout* m_layout;
	std::map<const std::string, DShaderParamDesc> m_params;
	std::map<const std::string, UINT> m_resParams;
	std::vector<ID3D10Buffer*> m_paramBuffers;
};

