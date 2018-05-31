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

int DShaderBuffer11::GetCBufferIndex(LPCSTR cbuffername) const
{
	if (m_paramIds.find(cbuffername) != m_paramIds.end()) 
	{
		return m_paramIds.at(cbuffername);
	}
	return -1;
}

HRESULT DShaderBuffer11::ApplyParam(void * buffer, int index)
{
	void* dataPtr;
	unsigned int bufferNumber;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	D3D11Core* core = (D3D11Core*)DSystem::GetGraphicsMgr()->GetGLCore();
	ID3D11Buffer* pbuffer = m_paramBuffers[index];

	HRESULT result = core->GetDeviceContext()->Map(pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (void*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	(&dataPtr) = &buffer;

	// Unlock the constant buffer.
	core->GetDeviceContext()->Unmap(pbuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	core->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &pbuffer);
}

void DShaderBuffer11::Release()
{
	int i;
	if (m_paramBuffers.size() > 0)
	{
		for (i = 0; i < m_paramBuffers.size(); i++)
		{
			ID3D11Buffer* buf = m_paramBuffers.at(i);
			if (buf != NULL)
			{
				buf->Release();
				buf = NULL;
			}
		}
		m_paramBuffers.clear();
	}
	m_paramIds.clear();

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
	result = InitShaderParams(vertexShaderBuffer, device, &m_layout, &byteLength);
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

HRESULT DShaderBuffer11::InitShaderParams(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout, int* inputLayoutByteLength)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
	D3D11_BUFFER_DESC paramBufferDesc;
	HRESULT hr = S_OK;
	m_cbufferCount = 0;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// get shader description
	D3D11_SHADER_DESC shaderDesc;
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

	for (unsigned int i = 0; i < shaderDesc.ConstantBuffers; ++i)
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

		m_paramIds[desc.Name] = m_paramBuffers.size();
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
