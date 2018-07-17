#pragma once
#include "DGeometry.h"
#include "DMaterial.h"
#include "DCamera.h"
#include "DImGUICore.h"
#include "DGLDrawer.h"
#include "DGraphicsDefine.h"

/*图形模块*/
class DGraphics
{
public:
	DGraphics();
	~DGraphics();
	/*初始化图形模块*/
	bool Init(int width /*屏幕宽度*/, int height /*屏幕高度*/, bool fullscreen /*全屏*/, HWND hwnd /*窗口句柄*/, DGraphicsAPI api);
	/*初始化窗口*/
	bool PreInitWindow(int width, int height, bool fullscreen, HWND&);
	/*帧函数*/
	bool Frame();
	/*关闭图形模块*/
	void Shutdown();
	DGLCore* GetGLCore();
	DImGUICore* GetGUICore();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void GetResolution(float&, float&);
	DGraphicsAPI GetAPI();

	/*开始场景绘制*/
	static void BeginScene(bool clearDepth, bool clearStencil, DColor& clearColor,  DRenderTexture* target = NULL);
	/*结束场景绘制*/
	static void EndScene(DRenderTexture* = NULL);
	/*清除缓冲区*/
	static void Clear(bool clearDepth, bool clearStencil, DColor& clearColor, DRenderTexture* target = NULL);
	/*设置渲染目标*/
	static void SetRenderTarget(DRenderTexture* = NULL);
	/*结束渲染*/
	static void EndSetRenderTarget(DRenderTexture* = NULL);
	/*立即绘制Geometry*/
	static void DrawGeometry(DGeometry*, const DMatrix4x4&, DMaterial*);
	//static void PushRenderCommand();
	/*绘制屏幕纹理*/
	static void DrawTexture(DTexture*, DMaterial*);
	/*绘制天空盒*/
	static void DrawSkyBox(DMaterial*, const DCamera*);
	
	static void SetCullMode(DCullMode);
	static void SetFillMode(DFillMode);
	static void SetZWriteEnable(bool);
	static void SetZTestFunc(DRSCompareFunc);
	static void SetBlendOp(DRSBlendOp);
	static void SetBlendEnable(bool);
	static void SetBlendSrcFactor(DRSBlendFactor);
	static void SetBlendDstFactor(DRSBlendFactor);

	/*设置视口区域*/
	static void SetViewPort(float x, float y, float width, float height);
	/*重置视口区域*/
	static void ResetViewPort();
	/*设置全局渲染shader*/
	static void SetGlobalRenderShader(DShader*);
	/*获取当前全局渲染shader*/
	static DShader* GetGlobalRenderShader();
	/*清除当前全局渲染shader*/
	static void ClearGlobalRenderShader();

	static void GlBegin();
	static void GlEnd();
	static void GlVector3(DVector3&);
	static void GlVector(float, float, float);
	/*设置当前绘制颜色*/
	static void GlColor(DColor&);
	/*对modelvewi矩阵和投影矩阵压栈*/
	static void GlPushMatrix();
	/*从栈顶取出modelview矩阵和投影矩阵*/
	static void GlPopMatrix();
	/*对当前modelview矩阵设置为单位矩阵*/
	static void GlLoadIndentity();
	/*设置投影矩阵*/
	static void GlLoadProjectionMatrix(DMatrix4x4&);
	/*将当前矩阵设置为正交投影*/
	static void GlLoadOrtho();
	/*对当前modelview矩阵右乘一个矩阵*/
	static void GlMultiMatrix(DMatrix4x4&);
	/*获取当前modelview矩阵*/
	static void GetModelView(DMatrix4x4&);
	/*获取当前投影矩阵*/
	static void GetProjection(DMatrix4x4&);

	static unsigned int GetDrawCall();

private:
	void InitScreenPlane();
	void InitSkyBox();

private:
	float* m_glVertices;
	unsigned int* m_glIndices;
	DGLCore* m_GL;
	DGraphicsAPI m_API;
	DImGUICore* m_GUI;
	DGLDrawer* m_glDrawer;
	DGeometry* m_screenPlane;
	DGeometry* m_skyGeometry;
	DMatrix4x4 m_modelView;
	DMatrix4x4 m_projection;
	DShader* m_globalRenderShader;

	unsigned int m_drawCall;
};

