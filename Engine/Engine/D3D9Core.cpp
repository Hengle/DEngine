#include "D3D9Core.h"
#include <fstream>
#include "DMeshRes9.h"
#include "DShaderRes9.h"

D3D9Core::D3D9Core()
{
	m_d3d = 0;
	m_device = 0;
}


D3D9Core::~D3D9Core()
{
}

bool D3D9Core::Init(int width, int height, bool fullScreen, HWND hwnd)
{
	DGLCore::Init(width, height, fullScreen, hwnd);

	HRESULT result;

	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DCAPS9 caps;
	m_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = !fullScreen;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = hwnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

								   // create a device class using this information and information from the d3dpp stuct
	result = m_d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		vp,
		&d3dpp,
		&m_device);

	if (FAILED(result))
	{
		MessageBox(hwnd, L"设备创建失败", L"初始化失败", MB_ERR_INVALID_CHARS);
		return false;
	}

	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.X = 0.0f;
	m_viewPort.Y = 0.0f;
	m_viewPort.MinZ = 0.0f;
	m_viewPort.MaxZ = 1.0f;

	return true;
}

void D3D9Core::Destroy()
{
	if (m_d3d != NULL)
	{
		m_d3d->Release();
		m_d3d = 0;
	}
	if (m_device != NULL)
	{
		m_device->Release();
		m_device = 0;
	}
}

void D3D9Core::BeginRender()
{
	m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 1.0f), 1.0f, 0);

	m_device->BeginScene();

	m_device->SetViewport(&m_viewPort);
}

void D3D9Core::EndRender()
{
	m_device->EndScene();

	m_device->Present(0, 0, 0, 0);
}

DMeshRes * D3D9Core::CreateMeshRes()
{
	DMeshRes9* res = new DMeshRes9(m_device);
	return res;
}

DTextureRes * D3D9Core::CreateTextureRes(WCHAR*)
{
	return nullptr;
}

DShaderRes * D3D9Core::CreateShaderRes()
{
	return new DShaderRes9(m_device);
}

void D3D9Core::ApplySamplerState(UINT, DWarpMode)
{
}
