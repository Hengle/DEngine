#pragma once
#include "D3D11Core.h"
#include "DMesh.h"
#include "DMaterial.h"
#include "DCamera.h"
//#include "DImGUI.h"

enum DGraphicsAPI
{
	DGRAPHICS_API_D3D11 = 0,
	DGRAPHICS_API_D3D10 = 1,
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
private:
	struct MatrixBufferType
	{
		DMatrix4x4 world;
		DMatrix4x4 view;
		DMatrix4x4 projection;
	};

	struct ViewBufferType
	{
		DVector3 camPos;
		float power;
		DColor color;
	};
public:
	DGraphics();
	~DGraphics();
	bool Init(int, int, bool, HWND, DGraphicsAPI);
	bool Render();
	void Shutdown();
	DGLCore* GetGLCore();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void GetResolution(FLOAT&, FLOAT&);
	DGraphicsAPI GetAPI();
	//ID3D11Device * GetDevice();
	//ID3D11DeviceContext* GetDeviceContext();

	static void DrawMesh(const DMesh*, const DMatrix4x4&, const DMaterial*, const DCamera*);

private:
	D3D11Core* m_D3D;
	DGraphicsAPI m_API;
	//DImGUI* m_GUI;
};

