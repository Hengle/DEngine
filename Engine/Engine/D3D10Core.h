#pragma once
#ifdef _DGAPI_D3D10

#include "DGLCore.h"
#include <d3d10_1.h>
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
	bool Init(int width, int height, bool fullscreen, HWND);
	/*模块销毁*/
	virtual void Destroy();
	/*提交渲染结果*/
	virtual void Present();
	/*清除缓冲区*/
	virtual void Clear(bool clearDepth /*清除深度缓冲*/, bool clearStencil /*清除模板缓冲*/, bool /*清除颜色缓冲*/, DColor& clearColor /*颜色缓冲区清除颜色*/, IRenderTextureViewRes* = NULL);
	/*设置渲染目标*/
	virtual void SetRenderTarget(IRenderTextureViewRes* = NULL);
	/*设置视口区域*/
	virtual void SetViewPort(float, float, float, float);
	/*结束渲染*/
	virtual void EndSetRenderTarget(IRenderTextureViewRes* = NULL);
	/*创建几何体资源*/
	virtual DGeometryRes* CreateGeometryRes(int vertexUsage, bool dynamic);
	/*创建贴图资源*/
	virtual ITextureRes* CreateTextureRes(WCHAR* path);
	/*创建RenderTexture资源*/
	virtual IRenderTextureViewRes* CreateRenderTextureRes(float width, float height);
	/*创建shader程序*/
	virtual DShaderPass* CreateShaderPass();
	virtual void ApplySamplerState(UINT, DWrapMode);
	virtual IRenderStateMgr* GetRenderStateMgr();

	virtual bool IsFrustrumZeroToOne() { return true; }
	ID3D10Device* GetDevice() const;

private:
	void InitSamplerStates();
	void InitRenderStateMgr();
	ID3D10SamplerState* CreateSamplerState(D3D10_TEXTURE_ADDRESS_MODE);

private:
	IDXGISwapChain* m_swapChain;
	ID3D10Device* m_device;
	ID3D10Texture2D* m_depthStencilBuffer;
	IRenderBuffer* m_colorBuffer;
	IRenderBuffer* m_depthBuffer;
	IRenderStateMgr* m_renderStateMgr;
	D3D10_VIEWPORT m_viewPort;
	std::map<DWrapMode, ID3D10SamplerState*> m_samplerStates;
};

#endif