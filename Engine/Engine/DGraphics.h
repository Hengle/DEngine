#pragma once
#include "DMesh.h"
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
	bool Init(int, int, bool, HWND, DGraphicsAPI);
	bool Execute();
	void Shutdown();
	DGLCore* GetGLCore();
	DImGUICore* GetGUICore();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void GetResolution(float&, float&);
	DGraphicsAPI GetAPI();

	static void BeginScene(bool, bool, DColor&, DRenderTexture* = NULL);
	static void EndScene(DRenderTexture* = NULL);
	static void Clear(bool, bool, DColor&, DRenderTexture* = NULL);
	static void SetRenderTarget(DRenderTexture* = NULL);
	static void EndSetRenderTarget(DRenderTexture* = NULL);

	static void DrawMesh(DMesh*, const DMatrix4x4&, DMaterial*, const DCamera*);
	static void DrawTexture(DTexture*, DMaterial*);
	static void DrawSkyBox(DMaterial*, const DCamera*);
	
	static void SetCullMode(DCullMode);
	static void SetZWriteEnable(bool);
	static void SetZTestFunc(DRSCompareFunc);
	static void SetBlendOp(DRSBlendOp);
	static void SetBlendEnable(bool);
	static void SetBlendSrcFactor(DRSBlendFactor);
	static void SetBlendDstFactor(DRSBlendFactor);

	static void SetViewPort(DRect&);

	static void GlBegin();
	static void GlEnd();
	static void GlVector3(DVector3&);
	static void GlVector(float, float, float);
	static void GlColor(DColor&);
	static void GlPushMatrix();
	static void GlPopMatrix();
	static void GlLoadIndentity();
	static void GlLoadProjectionMatrix(DMatrix4x4&);
	static void GlLoadOrtho();
	static void GlMultiMatrix(DMatrix4x4&);
	static void GetModelView(DMatrix4x4&);
	static void GetProjection(DMatrix4x4&);

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
	DMesh* m_screenPlane;
	DMesh* m_skyMesh;
	DMatrix4x4 m_modelView;
	DMatrix4x4 m_projection;
};

