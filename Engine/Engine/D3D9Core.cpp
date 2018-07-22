#ifdef _DGAPI_D3D9
#include "D3D9Core.h"
#include <fstream>
#include "DGeometryRes9.h"
#include "DShaderProgram9.h"
#include "DTextureRes9.h"
#include "DRenderStateMgr9.h"
#include <exception>

D3D9Core::D3D9Core()
{
	m_d3d = 0;
	m_device = 0;
	m_renderStateMgr = 0;
}


D3D9Core::~D3D9Core()
{
}

bool D3D9Core::Init(int width, int height, bool fullScreen, HWND hwnd)
{
	DGLCore::Init(width, height, fullScreen, hwnd);

	HRESULT result;

	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);    //创建direct3d9接口

	D3DCAPS9 caps;
	m_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));  
	d3dpp.Windowed = !fullScreen;   
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  
	d3dpp.hDeviceWindow = hwnd;  
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

	//创建设备接口
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

	InitRenderStateMgr();

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
	if (m_renderStateMgr != NULL)
	{
		m_renderStateMgr->Release();
		delete m_renderStateMgr;
		m_renderStateMgr = 0;
	}
}

void D3D9Core::Present()
{
	m_device->Present(0, 0, 0, 0);
}

void D3D9Core::Clear(bool clearDepth, bool clearStencil, bool clearColor, DColor & color, IRenderTextureViewRes * res)
{
	DWORD flag = 0;
	if (clearColor)
		flag |= D3DCLEAR_TARGET;
	if (clearDepth)
		flag |= D3DCLEAR_ZBUFFER;
	if (clearStencil)
		flag |= D3DCLEAR_STENCIL;
	D3DCOLOR c = D3DCOLOR_COLORVALUE(color.r, color.g, color.b, color.a);

	if (res != NULL)
	{
		DRenderTextureViewRes9* r = (DRenderTextureViewRes9*)res;
		LPD3DXRENDERTOSURFACE isurface = r->GetInterface();
		isurface->BeginScene(r->GetSurface(), &m_viewPort);
		m_device->Clear(0, NULL, flag, c, 1.0f, 0);
	}
	else
	{
		m_device->Clear(0, NULL, flag, c, 1.0f, 0);
		m_device->BeginScene();
	}
}

void D3D9Core::SetRenderTarget(IRenderTextureViewRes * res)
{
}

void D3D9Core::SetViewPort(float x, float y, float width, float height)
{
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.X = x;
	m_viewPort.Y = y;

	m_device->SetViewport(&m_viewPort);
}

void D3D9Core::EndSetRenderTarget(IRenderTextureViewRes * res)
{
	if (res != NULL)
	{
		DRenderTextureViewRes9* r = (DRenderTextureViewRes9*)res;
		LPD3DXRENDERTOSURFACE isurface = r->GetInterface();
		isurface->EndScene(0);
	}
	else
	{
		m_device->EndScene();
	}
}

DGeometryRes * D3D9Core::CreateGeometryRes(int vertexUsage, bool dynamic)
{
	DGeometryRes9* res = new DGeometryRes9(m_device, vertexUsage, dynamic);
	return res;
}

ITextureRes * D3D9Core::CreateTextureRes(WCHAR* filename)
{
	return new DTextureRes9(m_device, filename);
}

IRenderTextureViewRes * D3D9Core::CreateRenderTextureRes(float width, float height)
{
	return new DRenderTextureViewRes9(m_device, width, height);
}

DShaderProgram * D3D9Core::CreateShaderProgram(DShaderProgramType programType)
{
	if(programType == DShaderProgram_Vertex)
		return new DShaderVertexProgram9(m_device);
	else if (programType == DShaderProgram_Pixel)
		return new DShaderPixelProgram9(m_device);
	else
		throw std::exception("未知shader类型");
}

void D3D9Core::ApplySamplerState(UINT index, DWrapMode warpmode)
{
	DWORD rindex = index;
	m_device->SetSamplerState(rindex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(rindex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(rindex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	if (warpmode == DWrapMode_Clamp)
	{
		m_device->SetSamplerState(rindex, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_device->SetSamplerState(rindex, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_device->SetSamplerState(rindex, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
	}
	else if(warpmode == DWrapMode_Repeat)
	{
		m_device->SetSamplerState(rindex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_device->SetSamplerState(rindex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		m_device->SetSamplerState(rindex, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	}
	else
		throw std::exception("未知wrapmode类型");
	
}

IRenderStateMgr * D3D9Core::GetRenderStateMgr()
{
	return m_renderStateMgr;
}

void D3D9Core::InitRenderStateMgr()
{
	m_renderStateMgr = new DRenderStateMgr9(m_device);
	m_renderStateMgr->Init();
}
#endif