#pragma once
#include "DGLCore.h"
#include "DMath.h"
#include <d3d9.h>
#include <d3dx9.h>

/*
	D3D9渲染与管理模块
*/
class D3D9Core : public DGLCore
{
public:
	D3D9Core();
	~D3D9Core();

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

	LPDIRECT3DDEVICE9 GetDevice()
	{
		return m_device;
	}

private:
	void InitRenderStateMgr();

private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_device;
	D3DVIEWPORT9 m_viewPort;
	IRenderStateMgr* m_renderStateMgr;
};

