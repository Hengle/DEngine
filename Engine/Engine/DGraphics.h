#pragma once
#include "D3DCore.h"
#include "DImGUI.h"
#include "DSceneManager.h"
#include "DLogManager.h"
#include "DTime.h"

class DGraphics
{
public:
	DGraphics();
	~DGraphics();
	bool Init(int, int, bool, HWND);
	bool Render(DSceneManager*, DLogManager*, DTime*);
	void Shutdown();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void GetResolution(FLOAT&, FLOAT&);

private:
	D3DCore* m_D3D;
	DImGUI* m_GUI;
};

