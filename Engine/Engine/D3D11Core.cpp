#include "D3D11Core.h"
#include "DSystem.h"
#include <D3DX11.h>
#include <d3dcompiler.h>

DMeshBuffer11::DMeshBuffer11()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

void DMeshBuffer11::Init(ID3D11Device* device, int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = dataSize * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return;
	}
}

ID3D11Buffer * DMeshBuffer11::GetVertexBuffer()
{
	return m_vertexBuffer;
}

ID3D11Buffer * DMeshBuffer11::GetIndexBuffer()
{
	return m_indexBuffer;
}

void DMeshBuffer11::Release()
{
	if (m_vertexBuffer != NULL)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
	if (m_indexBuffer != NULL)
	{
		m_indexBuffer->Release();
		m_indexBuffer = NULL;
	}
}

DTextureBuffer11::DTextureBuffer11()
{
	m_resourceView = 0;
}

void DTextureBuffer11::Init(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result;
	result = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_resourceView, NULL);
	if (FAILED(result))
	{
		return;
	}
}

void DTextureBuffer11::Release()
{
	if (m_resourceView != NULL)
	{
		m_resourceView->Release();
		m_resourceView = NULL;
	}
}

D3D11Core::D3D11Core() : DGLCore()
{
	m_device = 0;
	m_deviceContext = 0;
	m_swapChain = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

D3D11Core::~D3D11Core()
{
}

bool D3D11Core::Init(int width, int height, bool fullScreen, HWND hwnd)
{
	DGLCore::Init(width, height, fullScreen, hwnd);
	IDXGIFactory *factory;
	HRESULT result;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

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

	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;

	return true;
}

void D3D11Core::Destroy()
{
	if (m_rasterState != NULL)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}
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
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
}

void D3D11Core::BeginRender(float r, float g, float b, float a)
{
	float color[4];
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_deviceContext->RSSetViewports(1, &m_viewPort);
}

void D3D11Core::EndRender()
{
	if (m_swapChain != NULL) {
		m_swapChain->Present(0, 0);
	}
}

DMeshBuffer * D3D11Core::CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float* vertices, const unsigned long* indices)
{
	DMeshBuffer11* buffer = new DMeshBuffer11();
	buffer->Init(m_device, vertexCount, indexCount, dataSize, vertices, indices);
	return buffer;
}

DTextureBuffer * D3D11Core::CreateTextureBuffer(WCHAR * fileName)
{
	DTextureBuffer11* buffer = new DTextureBuffer11();
	buffer->Init(m_device, fileName);
	return buffer;
}

DShaderBuffer * D3D11Core::CreateShaderBuffer(WCHAR * vertexShader, WCHAR * pixelShader)
{
	DShaderBuffer11* sbf = new DShaderBuffer11();
	sbf->Init(m_device, vertexShader, pixelShader);
	return sbf;
}

void D3D11Core::ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float* params)
{
	DShaderBuffer11* bf11 = (DShaderBuffer11*)shaderBuffer;
	bf11->ApplyBuffer(m_deviceContext, cindex, coffset, csize, stype, params);
}

void D3D11Core::ApplyTextureParams(DTextureBuffer * textureBuffer)
{
	//m_deviceContext->PSSetShaderResources()
}

void D3D11Core::DrawMesh(const DMeshBuffer * meshBuffer, int dataSize)
{
	unsigned int stride;
	unsigned int offset;


	stride = dataSize;
	offset = 0;

	ID3D11Buffer* vbuffer = ((DMeshBuffer11*)(meshBuffer))->GetVertexBuffer();
	ID3D11Buffer* ibuffer = ((DMeshBuffer11*)(meshBuffer))->GetIndexBuffer();

	m_deviceContext->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_deviceContext->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D11Core::DrawShader(const DShaderBuffer * buffer, int indexCount)
{
	((DShaderBuffer11*)buffer)->Draw(m_deviceContext, indexCount);
}

void D3D11Core::SetBackBufferRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

ID3D11Device * D3D11Core::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext * D3D11Core::GetDeviceContext()
{
	return m_deviceContext;
}

ID3D11DepthStencilView * D3D11Core::GetDepthStencilView()
{
	return m_depthStencilView;
}

DShaderBuffer11::DShaderBuffer11()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
}

DShaderBuffer11::~DShaderBuffer11()
{
}

void DShaderBuffer11::Init(ID3D11Device* device, WCHAR * vsFilename, WCHAR * psFilename)
{
	bool result;

	result = InitShader(device, vsFilename, psFilename);
}

unsigned int DShaderBuffer11::GetCBufferCount() const
{
	return m_cbufferCount;
}

unsigned int DShaderBuffer11::GetPropertyCount() const
{
	return m_propertyCount;
}

void DShaderBuffer11::GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int& poffset, int& plength, int& stype) const
{
	if (m_params.find(key) != m_params.end())
	{
		ShaderParam pm = m_params.at(key);
		//if (pm.properties.find(key) != pm.properties.end())
		//{
			//ShaderProperty pr = pm.properties.at(key);
		cindex = pm.bufferIndex;
		coffset = pm.bufferOffset;
		clength = pm.bufferLength;
		poffset = pm.paramOffset;
		plength = pm.paramLength;
		stype = pm.shaderType;
		return;
		//}
	}
	cindex = -1;
	coffset = -1;
	clength = 0;
	poffset = -1;
	plength = 0;
	stype = 0;
}

bool DShaderBuffer11::HasProperty(const LPCSTR key) const
{
	if (m_params.find(key) != m_params.end())
	{
		return true;
	}
	return false;
}

//unsigned int DShaderBuffer11::GetPropertyCount(LPCSTR cbuffername) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		return m_params.at(cbuffername).propertyCount;
//	}
//	return 0;
//}
//
//void DShaderBuffer11::GetCBufferInfo(LPCSTR cbuffername, int & index, int & offset, int& length, int& shaderType) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		//if (stype == shaderType)
//		{
//			index = m_params.at(cbuffername).bufferIndex;
//			offset = m_params.at(cbuffername).bufferOffset;
//			shaderType = m_params.at(cbuffername).shaderType;
//			length = m_params.at(cbuffername).bufferLength;
//			return;
//		}
//	}
//	index = -1;
//	offset = -1;
//	shaderType = 0;
//	length = 0;
//}
//
//int DShaderBuffer11::GetCBufferIndex(LPCSTR cbuffername) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		//if (m_params.at(cbuffername).shaderType == shaderType)
//			return m_params.at(cbuffername).bufferIndex;
//	}
//	return -1;
//}
//
//int DShaderBuffer11::GetCBufferOffset(LPCSTR cbuffername) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		//if (m_params.at(cbuffername).shaderType == shaderType)
//		return m_params.at(cbuffername).bufferOffset;
//	}
//	return -1;
//}
//
//int DShaderBuffer11::GetCBufferLength(LPCSTR cbuffername) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		//if (m_params.at(cbuffername).shaderType == shaderType)
//		return m_params.at(cbuffername).bufferLength;
//	}
//	return 0;
//}
//
//int DShaderBuffer11::GetCBufferType(LPCSTR cbuffername) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		//if (m_params.at(cbuffername).shaderType == shaderType)
//		return m_params.at(cbuffername).shaderType;
//	}
//	return 0;
//}
//
//int DShaderBuffer11::GetPropertyIndex(const LPCSTR cbuffername, const LPCSTR key) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		ShaderParam pm = m_params.at(cbuffername);
//		if (pm.properties.find(key) != pm.properties.end())
//			return pm.properties.at(key).index;
//	}
//	return -1;
//}
//
//int DShaderBuffer11::GetPropertyOffset(const LPCSTR cbuffername, const LPCSTR key) const
//{
//	if (m_params.find(cbuffername) != m_params.end())
//	{
//		ShaderParam pm = m_params.at(cbuffername);
//		if (pm.properties.find(key) != pm.properties.end())
//			return pm.properties.at(key).offset;
//	}
//	return -1;
//}
//
//int DShaderBuffer11::GetPropertyLength(const LPCSTR cbufferName, const LPCSTR key) const
//{
//	if (m_params.find(cbufferName) != m_params.end())
//	{
//		ShaderParam pm = m_params.at(cbufferName);
//		if (pm.properties.find(key) != pm.properties.end())
//			return pm.properties.at(key).length;
//	}
//	return 0;
//}
//
//void DShaderBuffer11::GetPropertyInfo(const LPCSTR cbufferName, const LPCSTR key, int & index, int & offset, int & length) const
//{
//	if (m_params.find(cbufferName) != m_params.end())
//	{
//		ShaderParam pm = m_params.at(cbufferName);
//		if (pm.properties.find(key) != pm.properties.end())
//		{
//			ShaderProperty pr = pm.properties.at(key);
//			index = pr.index;
//			offset = pr.offset;
//			length = pr.length;
//			return;
//		}
//	}
//	index = -1;
//	offset = -1;
//	length = 0;
//}

//DShaderParam * DShaderBuffer11::GetParams() const
//{
//	DShaderParam11* param = new DShaderParam11(m_cbufferCount);
//	
//	std::map<const std::string, ShaderParam>::const_iterator it;
//
//	it = m_params.begin();
//
//	int index, size;
//
//	D3D11Core* core = (D3D11Core*)DSystem::GetGraphicsMgr()->GetGLCore();
//
//	while (it != m_params.end())
//	{
//		//it->first;
//		//it->second;
//		index = it->second.index;
//		size = it->second.size;
//		param->SetParam(core->GetDevice(), index, size);
//		it++;
//	}
//
//	return param;
//}

void DShaderBuffer11::Release()
{
	int i;
	if (m_paramBuffers.size() > 0) 
	{
		for (i = 0; i < m_paramBuffers.size(); i++)
		{
			ID3D11Buffer* bf = m_paramBuffers.at(i);
			if (bf != NULL)
			{
				bf->Release();
				bf = NULL;
			}
		}
	}
	m_paramBuffers.clear();
	m_params.clear();

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void DShaderBuffer11::ApplyBuffer(ID3D11DeviceContext * deviceContext, int cindex, int coffset, int csize, int stype, float* params)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	float* dataPtr;
	unsigned int bufferNumber = coffset;

	ID3D11Buffer* pbuffer = m_paramBuffers[cindex];

	result = deviceContext->Map(pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return;
	}

	dataPtr = (float*)mappedResource.pData;

	int i;
	for (i = 0; i < csize; i++)
	{
		dataPtr[i] = params[i];
	}

	deviceContext->Unmap(pbuffer, 0);

	if (stype == 0)
	{
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &pbuffer);
	}
	else
	{
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &pbuffer);
	}
}

void DShaderBuffer11::Draw(ID3D11DeviceContext * deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}

//void DShaderBuffer11::SetParam(ID3D11DeviceContext * deviceContext, int index, void * value)
//{
//	ID3D11Buffer* bf = m_paramBuffers.at(index);
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//
//	result = deviceContext->Map(bf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		return;
//	}
//
//	void* dataPtr = mappedResource.pData;
//}

bool DShaderBuffer11::InitShader(ID3D11Device * device, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;


	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			//OutputShaderErrorMessage(errorMessage, vsFilename);
		}
		else
		{
			/*char s[1024];
			_bstr_t b(vsFilename);
			char* fname = b;
			sprintf_s(s, "Missing Vertex Shader File:%s", fname);
			DLog::Err(s);*/
		}

		return false; 
	}

	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			//OutputShaderErrorMessage(errorMessage, psFilename);
		}
		else
		{
			/*char s[1024];
			_bstr_t b(psFilename);
			char* fname = b;
			sprintf_s(s, "Missing Pixel Shader File:%s", fname);
			DLog::Err(s);*/
		}

		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout.

	int byteLength = 0;
	m_cbufferCount = 0;
	m_propertyCount = 0;

	result = InitVertexShader(vertexShaderBuffer, device, &m_layout, &byteLength);
	if (FAILED(result))
	{
		return false;
	}

	result = InitPixelShader(pixelShaderBuffer, device);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	/*char s[1024];
	_bstr_t b(vsFilename);
	char* fname = b;
	sprintf_s(s, "Shader Load Complie Success! (%s)", fname);
	DLog::Info(s);*/

	return true;
}

HRESULT DShaderBuffer11::InitVertexShader(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout, int* inputLayoutByteLength)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
	D3D11_BUFFER_DESC paramBufferDesc;
	HRESULT hr = S_OK;
	
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// get shader description
	D3D11_SHADER_DESC shaderDesc;

	ShaderParam param;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	unsigned int byteOffset = 0;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (unsigned int i = 0; i< shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);
		// create input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		//elementDesc.AlignedByteOffset = byteOffset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		LPCSTR sname = paramDesc.SemanticName;
		if (lstrcmpA(sname, "POSITION") == 0)
		{
			elementDesc.AlignedByteOffset = 0;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
		}
		else if (lstrcmpA(sname, "TEXCOORD") == 0)
		{
			elementDesc.AlignedByteOffset = 12;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			byteOffset += 8;
		}
		else if (lstrcmpA(sname, "NORMAL") == 0)
		{
			elementDesc.AlignedByteOffset = 20;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
		}
		else if (lstrcmpA(sname, "COLOR") == 0)
		{
			elementDesc.AlignedByteOffset = 32;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			byteOffset += 16;
		}
		//// determine DXGI format
		//if (paramDesc.Mask == 1)
		//{
		//	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		//	byteOffset += 4;
		//}
		//else if (paramDesc.Mask <= 3)
		//{
		//	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		//	byteOffset += 8;
		//}
		//else if (paramDesc.Mask <= 7)
		//{
		//	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		//	byteOffset += 12;
		//}
		//else if (paramDesc.Mask <= 15)
		//{
		//	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		//	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//	byteOffset += 16;
		//}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	int fsize = sizeof(float);
	int clength = 0;

	unsigned int i;

	for (i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* bf = pVertexShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC desc;
		bf->GetDesc(&desc);

		paramBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		paramBufferDesc.ByteWidth = desc.Size;
		paramBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		paramBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		paramBufferDesc.MiscFlags = 0;
		paramBufferDesc.StructureByteStride = 0;

		ID3D11Buffer* buffer;

		pD3DDevice->CreateBuffer(&paramBufferDesc, NULL, &buffer);

		clength = desc.Size / fsize;

		//m_paramIds[desc.Name] = m_paramBuffers.size();
		//m_params[desc.Name] = ShaderParam(m_cbufferCount, desc.Size);

		int offset = 0;
		int size = 0;

		for (unsigned int j = 0; j < desc.Variables; j++)
		{
			ID3D11ShaderReflectionVariable* bv = bf->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC vdesc;
			bv->GetDesc(&vdesc);

			offset = vdesc.StartOffset / fsize;
			size = vdesc.Size / fsize;

			param = ShaderParam(m_cbufferCount, i, clength, offset, size, 0);
			m_params.insert(std::pair<const LPCSTR, ShaderParam>(vdesc.Name, param));

			//param.properties.insert(std::pair<const LPCSTR, ShaderProperty>(vdesc.Name, ShaderProperty(j, size, offset)));

			m_propertyCount += 1;
		}
		
		m_paramBuffers.push_back(buffer);

		m_cbufferCount += 1;
	}
	
	// Try to create Input Layout
	hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);
	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	//record byte length
	*inputLayoutByteLength = byteOffset;
	return hr;
}

HRESULT DShaderBuffer11::InitPixelShader(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice)
{
	// Reflect shader info
	ID3D11ShaderReflection* pPixelShaderReflection = nullptr;
	
	HRESULT hr = S_OK;

	D3D11_SHADER_DESC shaderDesc;

	D3D11_BUFFER_DESC paramBufferDesc;

	ShaderParam param;

	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pPixelShaderReflection)))
	{
		return S_FALSE;
	}
	pPixelShaderReflection->GetDesc(&shaderDesc);

	int fsize = sizeof(float);

	int clength = 0;

	unsigned int i;
	
	for (i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* bf = pPixelShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC desc;
		bf->GetDesc(&desc);

		paramBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		paramBufferDesc.ByteWidth = desc.Size;
		paramBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		paramBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		paramBufferDesc.MiscFlags = 0;
		paramBufferDesc.StructureByteStride = 0;

		ID3D11Buffer* buffer;

		pD3DDevice->CreateBuffer(&paramBufferDesc, NULL, &buffer);

		clength = desc.Size / fsize;

		

		int offset = 0;
		int size = 0;

		for (unsigned int j = 0; j < desc.Variables; j++)
		{
			ID3D11ShaderReflectionVariable* bv = bf->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC vdesc;
			bv->GetDesc(&vdesc);

			offset = vdesc.StartOffset / fsize;
			size = vdesc.Size / fsize;

			param = ShaderParam(m_cbufferCount, i, clength, offset, size, 1);

			//param.properties.insert(std::pair<const LPCSTR, ShaderProperty>(vdesc.Name, ShaderProperty(j, size, offset)));

			m_params.insert(std::pair<const LPCSTR, ShaderParam>(vdesc.Name, param));

			m_propertyCount += 1;
		}

		
		m_paramBuffers.push_back(buffer);

		m_cbufferCount += 1;
	}

	pPixelShaderReflection->Release();
}

//DShaderParam11::DShaderParam11(int count)
//{
//	m_count = count;
//	m_paramsBuffer = new ID3D11Buffer*[count];
//}
//
//void DShaderParam11::SetParam(ID3D11Device * device, int index, int size)
//{
//	if (index < 0 || index >= m_count)
//		return;
//	D3D11_BUFFER_DESC paramBufferDesc;
//
//	paramBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	paramBufferDesc.ByteWidth = size;
//	paramBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	paramBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	paramBufferDesc.MiscFlags = 0;
//	paramBufferDesc.StructureByteStride = 0;
//
//	ID3D11Buffer* buffer;
//
//	device->CreateBuffer(&paramBufferDesc, NULL, &buffer);
//
//	m_paramsBuffer[index] = buffer;
//}
//
//void DShaderParam11::BeginSetParam(int id, void ** value)
//{
//	ID3D11Buffer* buffer = m_paramsBuffer[id];
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//
//	D3D11Core* core = (D3D11Core*)DSystem::GetGraphicsMgr()->GetGLCore();
//
//	unsigned int bufferNumber;
//
//	result = core->GetDeviceContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		return;
//	}
//
//	(*value) = (void*)mappedResource.pData;
//
//	
//}
//
//void DShaderParam11::EndSetParam(int id)
//{
//	D3D11Core* core = (D3D11Core*)DSystem::GetGraphicsMgr()->GetGLCore();
//	ID3D11Buffer* buffer = m_paramsBuffer[id];
//	core->GetDeviceContext()->Unmap(buffer, 0);
//
//	core->GetDeviceContext()->VSSetConstantBuffers(id, 1, &buffer);
//
//}
//
//void DShaderParam11::Release()
//{
//	int i;
//	for (i = 0; i < m_count; i++)
//	{
//		if (m_paramsBuffer[i] != NULL)
//		{
//			m_paramsBuffer[i]->Release();
//			m_paramsBuffer[i] = NULL;
//		}
//	}
//	delete[] m_paramsBuffer;
//	m_paramsBuffer = 0;
//}
