#pragma once
#include "DMesh.h"
#include "DMaterial.h"
#include "DCamera.h"
//#include "DImGUI.h"

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
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void GetResolution(float&, float&);
	DGraphicsAPI GetAPI();
	//ID3D11Device * GetDevice();
	//ID3D11DeviceContext* GetDeviceContext();

	static void DrawMesh(const DMesh*, const DMatrix4x4&, DMaterial*, const DCamera*);

private:
	DGLCore* m_GL;
	DGraphicsAPI m_API;
	//DImGUI* m_GUI;
};

