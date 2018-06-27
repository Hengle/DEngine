#pragma once
#include "DMesh.h"
#include "DMaterial.h"
#include "DCamera.h"
#include "DImGUICore.h"
#include "DGLDrawer.h"

enum DGraphicsAPI
{
	DGRAPHICS_API_D3D11 = 0,
	DGRAPHICS_API_D3D10 = 1,
	DGRAPHICS_API_D3D9  = 2,
};

enum DVertexBufferType
{
	DVERTEX_BUFFER_TYPE_POSITION = 1,
	DVERTEX_BUFFER_TYPE_NORMAL = 2,
	DVERTEX_BUFFER_TYPE_COLOR = 4,
	DVERTEX_BUFFER_TYPE_UV0 = 8,
	DVERTEX_BUFFER_TYPE_UV1 = 16,
	DVERTEX_BUFFER_TYPE_UV2 = 32,
};

class DGraphics
{
public:
	DGraphics();
	~DGraphics();
	bool Init(int, int, bool, HWND, DGraphicsAPI);
	bool Render();
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
	//static void Blit()
	static void SetCullMode(DCullMode);
	static void SetZWriteEnable(bool);
	static void SetZTestFunc(DRSCompareFunc);

	static void GlBegin();
	static void GlEnd();
	static void GLVector3(DVector3&);
	static void GLVector(float, float, float);
	static void GLColor(DColor&);
	static void GLPushMatrix();
	static void GLPopMatrix();
	static void GLLoadIndentity();

private:
	void InitScreenPlane();

private:
	float* m_glVertices;
	unsigned int* m_glIndices;
	DGLCore* m_GL;
	DGraphicsAPI m_API;
	DImGUICore* m_GUI;
	DGLDrawer* m_glDrawer;
	DMesh* m_screenPlane;
};

