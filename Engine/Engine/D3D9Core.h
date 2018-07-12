﻿#pragma once
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
	/*提交渲染结果*/
	virtual void Present();
	/*清除缓冲区*/
	virtual void Clear(bool clearDepth /*清除深度缓冲*/, bool clearStencil /*清除模板缓冲*/, DColor& clearColor /*颜色缓冲区清除颜色*/, IRenderTextureViewRes* = NULL);
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
	virtual DShaderProgram* CreateShaderProgram(DShaderProgramType);
	virtual void ApplySamplerState(UINT, DWrapMode);
	/*获取渲染状态管理器*/
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

