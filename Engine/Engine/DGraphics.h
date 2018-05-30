#pragma once
#include "DGLCore.h"
#include "DMesh.h"
//#include "DImGUI.h"

enum DGraphicsAPI
{
	DGRAPHICS_API_D3D11 = 0,
	DGRAPHICS_API_D3D10 = 1,
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
	void GetResolution(FLOAT&, FLOAT&);
	DGraphicsAPI GetAPI();
	ID3D11Device * GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	static void DrawMesh(DMesh*);

private:
	D3DCore11* m_D3D;
	DGraphicsAPI m_API;
	//DImGUI* m_GUI;
};

