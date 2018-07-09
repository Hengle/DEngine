#pragma once
#include "DGLCore.h"
#include <d3d10.h>
#include <map>

/*
	D3D10渲染与管理模块
*/
class D3D10Core : public DGLCore
{
public:
	D3D10Core();
	~D3D10Core();

	/*模块初始化*/
	virtual bool Init(int width, int height, bool fullscreen, HWND);
	/*模块销毁*/
	virtual void Destroy();
	//virtual void BeginRender();
	//virtual void EndRender();
	/*提交渲染结果*/
	virtual void Present();
	virtual void Clear(bool clearDepth, bool clearStencil, DColor& clearColor, DRenderTextureViewRes* = NULL);
	virtual void SetRenderTarget(DRenderTextureViewRes* = NULL);
	virtual void SetViewPort(float, float, float, float);
	virtual void EndSetRenderTarget(DRenderTextureViewRes* = NULL);
	/*创建网格资源*/
	virtual DMeshRes* CreateMeshRes(int vertexUsage, bool dynamic);
	/*创建贴图资源*/
	virtual DTextureRes* CreateTextureRes(WCHAR* path);
	/*创建RenderTexture资源*/
	virtual DRenderTextureViewRes* CreateRenderTextureRes(float width, float height);
	/*创建shader资源*/
	virtual DShaderRes* CreateShaderRes();
	virtual void ApplySamplerState(UINT, DWrapMode);
	virtual IRenderStateMgr* GetRenderStateMgr();
	ID3D10Device* GetDevice() const;

private:
	void InitSamplerStates();
	void InitRenderStateMgr();
	ID3D10SamplerState* CreateSamplerState(D3D10_TEXTURE_ADDRESS_MODE);

private:
	IDXGISwapChain* m_swapChain;
	ID3D10Device* m_device;
	ID3D10Texture2D* m_depthStencilBuffer;
	DRenderBuffer* m_colorBuffer;
	DRenderBuffer* m_depthBuffer;
	IRenderStateMgr* m_renderStateMgr;
	D3D10_VIEWPORT m_viewPort;
	std::map<DWrapMode, ID3D10SamplerState*> m_samplerStates;
};

