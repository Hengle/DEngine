#pragma once
#include "D3DCore.h"
#include "DImGUI.h"

enum DGraphicsAPI
{
	DGRAPHICS_API_D3D11 = 0,
	DGRAPHICS_API_D3D10 = 1,
};

class DGraphicsCore
{
public:
	DGraphicsCore();
	~DGraphicsCore();
	bool Init(int, int, bool, HWND, DGraphicsAPI);
	bool Render();
	void Shutdown();
	/*ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();*/
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void GetResolution(FLOAT&, FLOAT&);

private:
	D3DCore11* m_D3D;
	DImGUI* m_GUI;
};

