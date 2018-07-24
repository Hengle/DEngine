#pragma once
#ifdef _DGAPI_D3D11

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
	bool Init(int width, int height, bool fullscreen, HWND);
	/*模块销毁*/
	virtual void Destroy();
	/*提交渲染结果*/
	virtual void Present();
	/*清除缓冲区*/
	virtual void Clear(bool /*清除深度缓冲*/, bool /*清除模板缓冲*/, bool /*清除颜色缓冲*/, DColor& /*颜色缓冲区清除颜色*/, IRenderTextureViewRes* = NULL);
	/*设置渲染目标*/
	virtual void SetRenderTarget(IRenderTextureViewRes* = NULL);
	/*设置视口区域*/
	virtual void SetViewPort(float, float, float, float);
	/*结束渲染*/
	virtual void EndSetRenderTarget(IRenderTextureViewRes* = NULL);
	/*创建几何体资源*/
	virtual DGeometryRes* CreateGeometryRes(int vertexUsage, bool dynamic);
	/*创建贴图资源*/
	virtual ITextureRes* CreateTextureRes(WCHAR*);
	/*创建RenderTexture资源*/
	virtual IRenderTextureViewRes* CreateRenderTextureRes(float width, float height);
	/*创建shader程序*/
	virtual DShaderProgram* CreateShaderProgram(DShaderProgramType);
	virtual void ApplySamplerState(UINT, DWrapMode);
	/*获取渲染状态管理器*/
	virtual IRenderStateMgr* GetRenderStateMgr();

	virtual bool IsFrustrumZeroToOne() { return true; }
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

private:
	/*初始化采样器状态*/
	void InitSamplerStates();
	/*初始化渲染状态管理器*/
	void InitRenderStateMgr();
	ID3D11SamplerState* CreateSamplerState(D3D11_TEXTURE_ADDRESS_MODE);

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Texture2D* m_depthStencilBuffer;
	IRenderBuffer* m_colorBuffer;
	IRenderBuffer* m_depthBuffer;
	IRenderStateMgr* m_renderStateMgr;
	D3D11_VIEWPORT m_viewPort;
	std::map<DWrapMode, ID3D11SamplerState*> m_samplerStates;
	
};

#endif