#include "D3D9Core.h"


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

void D3D9Core::BeginRender(float r, float g, float b, float a)
{
	m_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(r, g, b, a), 1.0f, 0);

	m_device->BeginScene();
}

void D3D9Core::EndRender()
{
	m_device->EndScene();

	m_device->Present(NULL, NULL, NULL, NULL);
}

DMeshBuffer * D3D9Core::CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float * vertices, const unsigned long * indices)
{
	DMeshBuffer9* buffer = new DMeshBuffer9();
	buffer->Init(m_device, vertexCount, indexCount, dataSize, vertices, indices);
	return buffer;
}

DTextureBuffer * D3D9Core::CreateTextureBuffer(WCHAR * fileName)
{
	return nullptr;
}

DShaderBuffer * D3D9Core::CreateShaderBuffer(WCHAR * vertexShader, WCHAR * pixelShader)
{
	DShaderBuffer9* sbf = new DShaderBuffer9();
	sbf->Init(m_device, vertexShader, pixelShader);
	return sbf;
}

void D3D9Core::ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float * params)
{
}

void D3D9Core::ApplyTextureParams(DTextureBuffer * textureBuffer)
{
}

void D3D9Core::DrawMesh(const DMeshBuffer *, int)
{
}

void D3D9Core::DrawShader(const DShaderBuffer *, int)
{
}

void D3D9Core::SetBackBufferRenderTarget()
{
}

DMeshBuffer9::DMeshBuffer9()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

void DMeshBuffer9::Init(LPDIRECT3DDEVICE9 device, int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices)
{
	device->CreateVertexBuffer(dataSize, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &m_vertexBuffer, 0);

	//m_vertexBuffer->Lock()
}

void DMeshBuffer9::Release()
{
	if (m_vertexBuffer != NULL)
		m_vertexBuffer->Release();
	m_vertexBuffer = NULL;
	if (m_indexBuffer != NULL)
		m_indexBuffer->Release();
	m_indexBuffer = NULL;
}

void DTextureBuffer9::Release()
{
}

DShaderBuffer9::DShaderBuffer9()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_vertexConstable = 0;
	m_pixelConstable = 0;
}

DShaderBuffer9::~DShaderBuffer9()
{

}

void DShaderBuffer9::Init(LPDIRECT3DDEVICE9 device, WCHAR * vertexShader, WCHAR * pixelShader)
{
	InitShader(device, vertexShader, pixelShader);
}

unsigned int DShaderBuffer9::GetCBufferCount() const
{
	return 0;
}

unsigned int DShaderBuffer9::GetPropertyCount() const
{
	return 0;
}

void DShaderBuffer9::GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int & poffset, int & plength, int & stype) const
{
}

bool DShaderBuffer9::HasProperty(const LPCSTR key) const
{
	return false;
}

void DShaderBuffer9::Release()
{
	if (m_vertexShader != NULL)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}
	if (m_pixelShader != NULL)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}
	if (m_vertexConstable != NULL)
	{
		m_vertexConstable->Release();
		m_vertexConstable = NULL;
	}
	if (m_pixelConstable != NULL)
	{
		m_pixelConstable->Release();
		m_pixelConstable = NULL;
	}
}

bool DShaderBuffer9::InitShader(LPDIRECT3DDEVICE9, WCHAR* vsfile, WCHAR* psfile)
{
	HRESULT hr;
	ID3DXBuffer* vshader,*pshader = 0;
	ID3DXBuffer* errorBuffer = 0;
	hr = D3DXCompileShaderFromFile(
		vsfile,
		0,
		0,
		"Main", // entry point function name
		"vs_1_1",
		D3DXSHADER_DEBUG,
		&vshader,
		&errorBuffer,
		&m_vertexConstable);

	if (FAILED(hr))
	{
		return false;
	}

	hr = D3DXCompileShaderFromFile(
		psfile,
		0,
		0,
		"Main",
		"ps_1_1",
		D3DXSHADER_DEBUG,
		&pshader,
		&errorBuffer,
		&m_pixelConstable);

	if (FAILED(hr))
	{
		return false;
	}

	
}

HRESULT DShaderBuffer9::InitVertexShader(ID3DBlob*, LPDIRECT3DDEVICE9, ID3D11InputLayout**, int*)
{

}

HRESULT DShaderBuffer9::InitPixelShader(ID3DBlob*, LPDIRECT3DDEVICE9)
{

}
