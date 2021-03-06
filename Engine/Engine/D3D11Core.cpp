﻿#ifdef _DGAPI_D3D11
#include "D3D11Core.h"
#include "DSystem.h"
#include "DGeometryWrapper11.h"
//#include "DShaderProgram11.h"
#include "D3DShaderPass11.h"
#include "DTextureWrapper11.h"
#include "DRenderStateMgr11.h"
#include "DRenderBuffer11.h"
#include <D3DX11.h>
#include <d3dcompiler.h>

D3D11Core::D3D11Core() : DGLCore()
{
	m_device = 0;
	m_deviceContext = 0;
	m_swapChain = 0;
	//m_renderTargetView = 0;
	//m_depthStencilBuffer = 0;
	//m_depthStencilState = 0;
	//m_depthStencilView = 0;
	//m_depthBuffer = 0;
	//m_colorBuffer = 0;
	m_backBuffer = 0;
	m_renderStateMgr = 0;
}

D3D11Core::~D3D11Core()
{
}

bool D3D11Core::Init(int width, int height, bool fullScreen, HWND hwnd)
{
	DGLCore::Init(width, height, fullScreen);
	IDXGIFactory *factory;
	HRESULT result;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	

	//D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

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
	sd.Windowed = !fullScreen;
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

	/*m_colorBuffer = DColorBuffer11::Create(m_device, backBuffer, NULL);
	if (m_colorBuffer == NULL)
	{
		return false;
	}*/

	/*result = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}*/

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	ID3D11Texture2D* depthBuffer;
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &depthBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//// Initialize the description of the stencil state.
	//ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//// Set up the description of the stencil state.
	//depthStencilDesc.DepthEnable = true;
	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	//depthStencilDesc.StencilEnable = true;
	//depthStencilDesc.StencilReadMask = 0xFF;
	//depthStencilDesc.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing.
	//depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing.
	//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Create the depth stencil state.
	//result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//// Set the depth stencil state.
	//m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//// Initailze the depth stencil view.
	//ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//// Set up the depth stencil view description.
	//depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	/*result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}*/
	/*m_depthBuffer = DDepthBuffer11::Create(m_device, m_depthStencilBuffer, &depthStencilViewDesc);
	if (m_depthBuffer == NULL)
	{
		return false;
	}*/

	//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	m_backBuffer = new DRenderTextureViewWrapper11(m_device, m_deviceContext, backBuffer, depthBuffer);

	SetRenderTarget();

	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;

	InitSamplerStates();
	InitRenderStateMgr();

	/*InitRenderTargetView(backBuffer);*/

	backBuffer->Release();
	backBuffer = 0;

	return true;
}

void D3D11Core::Destroy()
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
	/*if (m_colorBuffer != NULL)
	{
		m_colorBuffer->Release();
		delete m_colorBuffer;
		m_colorBuffer = 0;
	}
	if (m_depthBuffer != NULL)
	{
		m_depthBuffer->Release();
		delete m_depthBuffer;
		m_depthBuffer = 0;
	}*/
	/*if (m_backBuffer != NULL)
	{
		m_backBuffer->Release();
		delete m_backBuffer;
		m_backBuffer = 0;
	}*/
	/*if (m_renderTargetView != NULL)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}*/

	/*if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}*/

	/*if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}*/
	if (m_renderStateMgr)
	{
		m_renderStateMgr->Release();
		delete m_renderStateMgr;
		m_renderStateMgr = 0;
	}
	/*if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}*/
	std::map<DWrapMode, ID3D11SamplerState*>::iterator  iter;
	for (iter = m_samplerStates.begin(); iter != m_samplerStates.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
		}
	}
	m_samplerStates.clear();
}

//void D3D11Core::BeginRender()
//{
//	/*float color[4];
//	color[0] = 0.0f;
//	color[1] = 0.0f;
//	color[2] = 1.0f;
//	color[3] = 1.0f;
//
//	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
//
//	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);*/
//
//	m_deviceContext->RSSetViewports(1, &m_viewPort);
//}
//
//void D3D11Core::EndRender()
//{
//	if (m_swapChain != NULL) {
//		m_swapChain->Present(0, 0);
//	}
//}

void D3D11Core::Present()
{
	if (m_swapChain != NULL) {
		m_swapChain->Present(0, 0);
	}
}

void D3D11Core::Clear(bool clearDepth, bool clearStencil, bool clearColor, DColor & color, IRenderTextureViewWrapper * renderTexture)
{
	int flag = 0;
	if (clearDepth && clearStencil)
		flag = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
	else if (clearDepth && !clearStencil)
		flag = D3D11_CLEAR_DEPTH;
	else if (!clearDepth && clearStencil)
		flag = D3D11_CLEAR_STENCIL;
	float c[4];
	c[0] = color.r;
	c[1] = color.g;
	c[2] = color.b;
	c[3] = color.a;

	if (renderTexture == NULL)
		renderTexture = m_backBuffer;

	//if (res != NULL)
	{
		DColorBuffer11*cb = (DColorBuffer11*)(renderTexture->GetColorBuffer());
		DDepthBuffer11*db = (DDepthBuffer11*)(renderTexture->GetDepthBuffer());
		if (flag != 0)
			m_deviceContext->ClearDepthStencilView(db->GetView(), flag, 1.0f, 0);
		if (clearColor)
			m_deviceContext->ClearRenderTargetView(cb->GetView(), c);
	}
	/*else
	{
		if (flag != 0)
			m_deviceContext->ClearDepthStencilView(((DDepthBuffer11*)m_depthBuffer)->GetView(), flag, 1.0f, 0);
		if (clearColor)
			m_deviceContext->ClearRenderTargetView(((DColorBuffer11*)m_colorBuffer)->GetView(), c);
	}*/
}

void D3D11Core::SetRenderTarget(IRenderTextureViewWrapper * renderTexture)
{
	if (renderTexture == NULL)
		renderTexture = m_backBuffer;
	//if (res != NULL)
	{
		DColorBuffer11*cb = (DColorBuffer11*)(renderTexture->GetColorBuffer());
		DDepthBuffer11*db = (DDepthBuffer11*)(renderTexture->GetDepthBuffer());
		ID3D11RenderTargetView* rv = cb->GetView();
		ID3D11DepthStencilView* dv = db->GetView();
		m_deviceContext->OMSetRenderTargets(1, &rv, dv);
	}
	/*else
	{
		ID3D11RenderTargetView* rv = ((DColorBuffer11*)m_colorBuffer)->GetView();
		ID3D11DepthStencilView* dv = ((DDepthBuffer11*)m_depthBuffer)->GetView();
		m_deviceContext->OMSetRenderTargets(1, &rv, dv);
	}*/
}

void D3D11Core::SetViewPort(float x, float y, float width, float height)
{
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.TopLeftX = x;
	m_viewPort.TopLeftY = y;

	m_deviceContext->RSSetViewports(1, &m_viewPort);
}

void D3D11Core::EndSetRenderTarget(IRenderTextureViewWrapper *)
{
	DColorBuffer11*cb = (DColorBuffer11*)(m_backBuffer->GetColorBuffer());
	DDepthBuffer11*db = (DDepthBuffer11*)(m_backBuffer->GetDepthBuffer());
	ID3D11RenderTargetView* rv = cb->GetView();
	ID3D11DepthStencilView* dv = db->GetView();
	m_deviceContext->OMSetRenderTargets(1, &rv, dv);
}

DGeometryWrapper * D3D11Core::CreateGeometryWrapper(int vertexUsage, bool dynamic)
{
	DGeometryWrapper11* wrapper = new DGeometryWrapper11(m_device, m_deviceContext, vertexUsage, dynamic);
	return wrapper;
}

ITextureWrapper * D3D11Core::CreateTextureWrapper(WCHAR* filename)
{
	return new DTextureWrapper11(m_device, m_deviceContext, filename);
}

ITextureWrapper * D3D11Core::CreateCubeMapWrapper(ITextureWrapper * right, ITextureWrapper * left, ITextureWrapper * top, ITextureWrapper * bottom, ITextureWrapper * front, ITextureWrapper * back)
{
	return new DTextureWrapper11(m_device, m_deviceContext, (DTextureWrapper11*)right, (DTextureWrapper11*)left, (DTextureWrapper11*)top, (DTextureWrapper11*)bottom, (DTextureWrapper11*)front, (DTextureWrapper11*)back);
}

IRenderTextureViewWrapper * D3D11Core::CreateRenderTextureWrapper(float width, float height)
{
	return new DRenderTextureViewWrapper11(m_device, m_deviceContext, width, height);
}

//DShaderProgram * D3D11Core::CreateShaderProgram(DShaderProgramType programType)
//{
//	if(programType == DShaderProgram_Vertex)
//		return new DShaderVertexProgram11(m_device, m_deviceContext);
//	else if(programType == DShaderProgram_Pixel)
//		return new DShaderPixelProgram11(m_device, m_deviceContext);
//}

DShaderPass * D3D11Core::CreateShaderPass()
{
	return new D3DShaderPass11();
}

void D3D11Core::ApplySamplerState(UINT startSlot, DWrapMode mode)
{
	ID3D11SamplerState* state = m_samplerStates.at(mode);
	if (state != NULL)
	{
		m_deviceContext->PSSetSamplers(startSlot, 1, &state);
	}
}

IRenderStateMgr * D3D11Core::GetRenderStateMgr()
{
	return m_renderStateMgr;
}

ID3D11Device * D3D11Core::GetDevice() const
{
	return m_device;
}

ID3D11DeviceContext * D3D11Core::GetDeviceContext() const
{
	return m_deviceContext;
}

void D3D11Core::InitSamplerStates()
{
	ID3D11SamplerState* state = CreateSamplerState(D3D11_TEXTURE_ADDRESS_WRAP);
	if (state != NULL)
		m_samplerStates.insert(std::pair<DWrapMode, ID3D11SamplerState*>(DWrapMode_Repeat, state));

	state = CreateSamplerState(D3D11_TEXTURE_ADDRESS_CLAMP);
	if (state != NULL)
		m_samplerStates.insert(std::pair<DWrapMode, ID3D11SamplerState*>(DWrapMode_Clamp, state));
}

void D3D11Core::InitRenderStateMgr()
{
	m_renderStateMgr = new DRenderStateMgr11(m_device, m_deviceContext);
	m_renderStateMgr->Init();
}

//void D3D11Core::InitRenderTargetView(ID3D11Texture2D * backBuffer)
//{
//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	shaderResourceViewDesc.Texture2D.MipLevels = 1;
//
//	HRESULT result = m_device->CreateShaderResourceView(backBuffer, &shaderResourceViewDesc, &m_renderTargetView);
//	if (FAILED(result))
//		return;
//}

ID3D11SamplerState* D3D11Core::CreateSamplerState(D3D11_TEXTURE_ADDRESS_MODE mode)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = mode;
	samplerDesc.AddressV = mode;
	samplerDesc.AddressW = mode;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT result;
	ID3D11SamplerState* state = 0;
	result = m_device->CreateSamplerState(&samplerDesc, &state);
	if (FAILED(result))
		return NULL;
	return state;
}
#endif