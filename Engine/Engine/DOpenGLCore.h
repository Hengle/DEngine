﻿#pragma once
#ifdef _DGAPI_OPENGL

#include "DGLCore.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class DOpenGLCore : public DGLCore
{
public:
	DOpenGLCore();
	~DOpenGLCore();

	/*模块初始化*/
	bool Init(int width, int height, bool fullscreen, GLFWwindow*);
	/*模块销毁*/
	virtual void Destroy();
	/*提交渲染结果*/
	virtual void Present();
	/*清除缓冲区*/
	virtual void Clear(bool /*清除深度缓冲*/, bool /*清除模板缓冲*/, bool /*清除颜色缓冲*/, DColor& /*颜色缓冲区清除颜色*/, IRenderTextureViewWrapper* = NULL);
	/*设置渲染目标*/
	virtual void SetRenderTarget(IRenderTextureViewWrapper* = NULL);
	/*设置视口区域*/
	virtual void SetViewPort(float, float, float, float);
	/*结束渲染*/
	virtual void EndSetRenderTarget(IRenderTextureViewWrapper* = NULL);
	/*创建几何体包装器*/
	virtual DGeometryWrapper* CreateGeometryWrapper(int vertexUsage, bool dynamic);
	/*创建贴图包装器*/
	virtual ITextureWrapper* CreateTextureWrapper(WCHAR*);
	virtual ITextureWrapper* CreateCubeMapWrapper(ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*, ITextureWrapper*);
	/*创建RenderTexture包装器*/
	virtual IRenderTextureViewWrapper* CreateRenderTextureWrapper(float width, float height);

	/*创建shader程序*/
	//virtual DShaderProgram* CreateShaderProgram(DShaderProgramType);
	virtual DShaderPass* CreateShaderPass();
	virtual void ApplySamplerState(UINT, DWrapMode);
	/*获取渲染状态管理器*/
	virtual IRenderStateMgr* GetRenderStateMgr();

	virtual bool IsFrustrumZeroToOne() { return false; }
	virtual bool IsUVStartsAtTop() { return false; }

private:
	///*初始化采样器状态*/
	//void InitSamplerStates();
	/*初始化渲染状态管理器*/
	void InitRenderStateMgr();
	//ID3D11SamplerState* CreateSamplerState(D3D11_TEXTURE_ADDRESS_MODE);

private:
	GLFWwindow* m_window;
	IRenderStateMgr* m_renderStateMgr;
};

#endif