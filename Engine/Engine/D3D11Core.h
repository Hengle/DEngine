#pragma once
#include "DGLCore.h"
#include <dxgi.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <map>

/*
	D3D11渲染与管理模块
*/
class D3D11Core : public DGLCore
{
public:
	D3D11Core();
	~D3D11Core();

	/*模块初始化*/
	virtual bool Init(int width, int height, bool fullscreen, HWND);
	/*模块销毁*/
	virtual void Destroy();
	//virtual void BeginRender();
	//virtual void EndRender();
	/*提交渲染结果*/
	virtual void Present();
	virtual void Clear(bool, bool, DColor&, DRenderTextureViewRes* = NULL);
	virtual void SetRenderTarget(DRenderTextureViewRes* = NULL);
	virtual void SetViewPort(DRect&);
	virtual void EndSetRenderTarget(DRenderTextureViewRes* = NULL);
	/*创建网格资源*/
	virtual DMeshRes* CreateMeshRes(int vertexUsage, bool dynamic);
	/*创建贴图资源*/
	virtual DTextureRes* CreateTextureRes(WCHAR*);
	/*创建RenderTexture资源*/
	virtual DRenderTextureViewRes* CreateRenderTextureRes(float width, float height);
	/*创建shader资源*/
	virtual DShaderRes* CreateShaderRes();
	virtual void ApplySamplerState(UINT, DWrapMode);
	virtual IRenderStateMgr* GetRenderStateMgr();
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

private:
	void InitSamplerStates();
	void InitRenderStateMgr();
	ID3D11SamplerState* CreateSamplerState(D3D11_TEXTURE_ADDRESS_MODE);

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Texture2D* m_depthStencilBuffer;
	DRenderBuffer* m_colorBuffer;
	DRenderBuffer* m_depthBuffer;
	IRenderStateMgr* m_renderStateMgr;
	D3D11_VIEWPORT m_viewPort;
	std::map<DWrapMode, ID3D11SamplerState*> m_samplerStates;
	
};
