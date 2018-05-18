#include "D3DCore.h"


D3DCore::D3DCore()
{
	m_device = 0;
	m_deviceContext = 0;
	m_swapChain = 0;
	m_renderTargetView = 0;
}


D3DCore::~D3DCore()
{
}

bool D3DCore::Init(int width, int height, bool fullScreen, HWND hwnd)
{
	IDXGIFactory *factory;
	HRESULT result;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	int i, numerator, denominator;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"创建IDXGIFactory失败", L"初始化失败", MB_ERR_INVALID_CHARS);
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"枚举适配器失败", L"初始化失败", MB_ERR_INVALID_CHARS);
		return false;
	}

	result = adapter->EnumOutputs(0, &output);
	if (FAILED(result)) 
	{
		MessageBox(hwnd, L"枚举输出设备失败", L"初始化失败", MB_ERR_INVALID_CHARS);
		return false;
	}

	result = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) 
	{
		return false;
	}

	displayModes = new DXGI_MODE_DESC[numModes];

	result = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);
	if (FAILED(result)) 
	{
		return false;
	}

	for (i = 0; i<numModes; i++)
	{
		if (displayModes[i].Width == (unsigned int)width)
		{
			if (displayModes[i].Height == (unsigned int)height)
			{
				numerator = displayModes[i].RefreshRate.Numerator;
				denominator = displayModes[i].RefreshRate.Denominator;
			}
		}
	}

	delete[] displayModes;
	displayModes = 0;

	output->Release();
	output = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = format;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	sd.OutputWindow = hwnd;

	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	sd.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result)) 
	{
		return false;
	}

	ID3D11Texture2D* backBuffer;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	backBuffer->Release();
	backBuffer = 0;

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);

	m_width = width;
	m_height = height;

	return true;
}

void D3DCore::Destroy()
{
	if (m_device != NULL)
	{
		m_device->Release();
		m_device = 0;
	}
	if (m_deviceContext != NULL)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}
	if (m_swapChain != NULL)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
	if (m_renderTargetView != NULL) 
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}
}

void D3DCore::BeginRender(float r, float g, float b, float a)
{
	float color[4];
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
}

void D3DCore::EndRender()
{
	if (m_swapChain != NULL) {
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device * D3DCore::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext * D3DCore::GetDeviceContext()
{
	return m_deviceContext;
}

void D3DCore::GetResolution(FLOAT &width, FLOAT &height)
{
	width = m_width;
	height = m_height;
}
