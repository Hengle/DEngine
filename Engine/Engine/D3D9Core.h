﻿#pragma once
#ifdef _DGAPI_D3D9

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
	bool Init(int width, int height, bool fullscreen, HWND);
	/*模块销毁*/
	virtual void Destroy();
	/*提交渲染结果*/
	virtual void Present();
	/*清除缓冲区*/
	virtual void Clear(bool clearDepth /*清除深度缓冲*/, bool clearStencil /*清除模板缓冲*/, bool /*清除颜色缓冲*/, DColor& clearColor /*颜色缓冲区清除颜色*/, IRenderTextureViewWrapper* = NULL);
	/*设置渲染目标*/
	virtual void SetRenderTarget(IRenderTextureViewWrapper* = NULL);
	/*设置视口区域*/
	virtual void SetViewPort(float, float, float, float);
	/*结束渲染*/
	virtual void EndSetRenderTarget(IRenderTextureViewWrapper* = NULL);
	/*创建几何体包装器*/
	virtual DGeometryWrapper* CreateGeometryWrapper(int vertexUsage, bool dynamic);
	/*创建贴图包装器*/
	virtual ITextureWrapper* CreateTextureWrapper(WCHAR* path);
	virtual ITextureWrapper* CreateCubeMapWrapper(ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*);
	/*创建RenderTexture包装器*/
	virtual IRenderTextureViewWrapper* CreateRenderTextureWrapper(float width, float height);

	/*创建shader程序*/
	virtual DShaderPass* CreateShaderPass();
	virtual void ApplySamplerState(UINT, DWrapMode);
	/*获取渲染状态管理器*/
	virtual IRenderStateMgr* GetRenderStateMgr();

	virtual bool IsFrustrumZeroToOne() { return true; }
	virtual bool IsUVStartsAtTop() { return true; }

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
	IDirect3DSurface9* m_backBuffer;
};

#endif