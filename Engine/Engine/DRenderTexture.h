#pragma once

#include "DTexture.h"

class DRenderTexture : public DTexture
{
public:
	DRenderTexture(int, int);
	~DRenderTexture();
	virtual bool Init(ID3D11Device*);
	virtual void Destroy();
	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* GetTexture();
	int GetWidth();
	int GetHeight();

private:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	int m_width, m_height;
};

