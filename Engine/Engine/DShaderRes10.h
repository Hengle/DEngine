#pragma once
#include "D3D10Core.h"
#include <string>
#include <vector>
#include <map>

/*
	Shader资源Direct3D10底层
*/
class DShaderRes10 : public DShaderRes
{
public:
	DShaderRes10(ID3D10Device* device);
	~DShaderRes10();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

private:
	HRESULT InitVertexShader(ID3DBlob*, ID3D10Device*, ID3D10InputLayout**, int*);
	HRESULT InitPixelShader(ID3DBlob*, ID3D10Device*);

protected:
	virtual bool OnInit(const char* content, char* vsfunc, char* psfunc);
	virtual void OnDraw();
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);

private:
	ID3D10Device* m_device;
	ID3D10VertexShader* m_vertexShader;
	ID3D10PixelShader *m_pixelShader;
	ID3D10InputLayout* m_layout;
	std::vector<DShaderResDesc> m_resParams;
	std::vector<DShaderCBufferDesc*> m_cbuffers;
	std::vector<ID3D10Buffer*> m_paramBuffers;
};

