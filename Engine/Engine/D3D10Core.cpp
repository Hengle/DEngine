#include "D3D10Core.h"

#include <d3dcompiler.h>
#include <D3DX10.h>

D3D10Core::D3D10Core()
{
	m_device = 0;
	m_swapChain = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}


D3D10Core::~D3D10Core()
{
}

bool D3D10Core::Init(int width, int height, bool fullScreen, HWND hwnd)
{
	DGLCore::Init(width, height, fullScreen, hwnd);
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	D3D10_TEXTURE2D_DESC  depthBufferDesc;
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D10_RASTERIZER_DESC rasterDesc;

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

	result = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION,
		&sd, &m_swapChain, &m_device);
	if (FAILED(result))
	{
		return false;
	}

	ID3D10Texture2D* backBuffer;

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
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
	depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
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
	depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_device->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	m_device->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D10_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D10_FILL_SOLID;
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
	m_device->RSSetState(m_rasterState);

	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
}

void D3D10Core::Destroy()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}


	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
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

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	return;
}

void D3D10Core::BeginRender(float r, float g, float b, float a)
{
	float color[4];
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	m_device->ClearRenderTargetView(m_renderTargetView, color);

	m_device->ClearDepthStencilView(m_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

	m_device->RSSetViewports(1, &m_viewPort);
}

void D3D10Core::EndRender()
{
	m_swapChain->Present(0, 0);
}

DMeshBuffer * D3D10Core::CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float * vertices, const unsigned long * indices)
{
	DMeshBuffer10* buffer = new DMeshBuffer10();
	buffer->Init(m_device, vertexCount, indexCount, dataSize, vertices, indices);
	return buffer;
}

DTextureBuffer * D3D10Core::CreateTextureBuffer(WCHAR * fileName)
{
	return nullptr;
}

DShaderBuffer * D3D10Core::CreateShaderBuffer(WCHAR * vertexShader, WCHAR * pixelShader)
{
	DShaderBuffer10* sbf = new DShaderBuffer10();
	sbf->Init(m_device, vertexShader, pixelShader);
	return sbf;
}

void D3D10Core::ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float * params)
{
	DShaderBuffer10* bf10 = (DShaderBuffer10*)shaderBuffer;
	bf10->ApplyBuffer(m_device, cindex, coffset, csize, stype, params);
}

void D3D10Core::ApplyTextureParams(DTextureBuffer * textureBuffer)
{
}

void D3D10Core::DrawMesh(const DMeshBuffer * meshBuffer, int dataSize)
{
	unsigned int stride;
	unsigned int offset;


	stride = dataSize;
	offset = 0;

	ID3D10Buffer* vbuffer = ((DMeshBuffer10*)(meshBuffer))->GetVertexBuffer();
	ID3D10Buffer* ibuffer = ((DMeshBuffer10*)(meshBuffer))->GetIndexBuffer();

	m_device->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_device->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D10Core::DrawShader(const DShaderBuffer * buffer, int indexCount)
{
	((DShaderBuffer10*)buffer)->Draw(m_device, indexCount);
}

void D3D10Core::SetBackBufferRenderTarget()
{
}

DMeshBuffer10::DMeshBuffer10()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

void DMeshBuffer10::Init(ID3D10Device * device, int vertexCount, int indexCount, int dataSize, const float * vertices, const unsigned long * indices)
{
	D3D10_BUFFER_DESC  vertexBufferDesc, indexBufferDesc;
	D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = dataSize * vertexCount;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	vertexData.pSysMem = vertices;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return;
	}

	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	indexData.pSysMem = indices;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return;
	}
}

ID3D10Buffer * DMeshBuffer10::GetVertexBuffer()
{
	return m_vertexBuffer;
}

ID3D10Buffer * DMeshBuffer10::GetIndexBuffer()
{
	return m_indexBuffer;
}

void DMeshBuffer10::Release()
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

DShaderBuffer10::DShaderBuffer10()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
}

DShaderBuffer10::~DShaderBuffer10()
{
}

void DShaderBuffer10::Init(ID3D10Device * device, WCHAR * vertexshader, WCHAR * pixelshader)
{
	InitShader(device, vertexshader, pixelshader);
}

unsigned int DShaderBuffer10::GetCBufferCount() const
{
	return m_cbufferCount;
}

unsigned int DShaderBuffer10::GetPropertyCount() const
{
	return m_propertyCount;
}

void DShaderBuffer10::GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int & poffset, int & plength, int & stype) const
{
	if (m_params.find(key) != m_params.end())
	{
		ShaderParam10 pm = m_params.at(key);
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

bool DShaderBuffer10::HasProperty(const LPCSTR key) const
{
	if (m_params.find(key) != m_params.end())
	{
		return true;
	}
	return false;
}

void DShaderBuffer10::Release()
{
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

void DShaderBuffer10::ApplyBuffer(ID3D10Device * device, int cindex, int coffset, int csize, int stype, float * params)
{
	HRESULT result;

	float* dataPtr;
	unsigned int bufferNumber = coffset;

	ID3D10Buffer* pbuffer = m_paramBuffers[cindex];

	result = pbuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&dataPtr);
	if (FAILED(result))
	{
		return;
	}

	int i;
	for (i = 0; i < csize; i++)
	{
		dataPtr[i] = params[i];
	}

	pbuffer->Unmap();

	if (stype == 0)
	{
		device->VSSetConstantBuffers(bufferNumber, 1, &pbuffer);
	}
	else
	{
		device->PSSetConstantBuffers(bufferNumber, 1, &pbuffer);
	}
}

void DShaderBuffer10::Draw(ID3D10Device *device, int indexCount)
{
	device->IASetInputLayout(m_layout);

	device->VSSetShader(m_vertexShader);
	device->PSSetShader(m_pixelShader);

	//deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	device->DrawIndexed(indexCount, 0, 0);
}

bool DShaderBuffer10::InitShader(ID3D10Device * device, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;


	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DX10CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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

	result = D3DX10CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), &m_pixelShader);
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

HRESULT DShaderBuffer10::InitVertexShader(ID3DBlob * pShaderBlob, ID3D10Device *pD3DDevice, ID3D10InputLayout **pInputLayout, int * inputLayoutByteLength)
{
	ID3D10ShaderReflection* pVertexShaderReflection = nullptr;
	D3D10_BUFFER_DESC paramBufferDesc;
	HRESULT hr = S_OK;


	if (FAILED(D3D10ReflectShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// get shader description
	D3D10_SHADER_DESC shaderDesc;

	ShaderParam10 param;

	//ShaderParam param;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	unsigned int byteOffset = 0;
	std::vector<D3D10_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (unsigned int i = 0; i< shaderDesc.InputParameters; ++i)
	{
		D3D10_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);
		// create input element desc
		D3D10_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		//elementDesc.AlignedByteOffset = byteOffset;
		elementDesc.InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
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

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	int fsize = sizeof(float);
	int clength = 0;

	unsigned int i;

	for (i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D10ShaderReflectionConstantBuffer* bf = pVertexShaderReflection->GetConstantBufferByIndex(i);
		D3D10_SHADER_BUFFER_DESC desc;
		bf->GetDesc(&desc);

		paramBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		paramBufferDesc.ByteWidth = desc.Size;
		paramBufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
		paramBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		paramBufferDesc.MiscFlags = 0;

		ID3D10Buffer* buffer;

		pD3DDevice->CreateBuffer(&paramBufferDesc, NULL, &buffer);

		clength = desc.Size / fsize;

		//m_paramIds[desc.Name] = m_paramBuffers.size();
		//m_params[desc.Name] = ShaderParam(m_cbufferCount, desc.Size);

		int offset = 0;
		int size = 0;

		for (unsigned int j = 0; j < desc.Variables; j++)
		{
			ID3D10ShaderReflectionVariable* bv = bf->GetVariableByIndex(j);

			D3D10_SHADER_VARIABLE_DESC vdesc;
			bv->GetDesc(&vdesc);

			offset = vdesc.StartOffset / fsize;
			size = vdesc.Size / fsize;

			param = ShaderParam10(m_cbufferCount, i, clength, offset, size, 0);
			m_params.insert(std::pair<const LPCSTR, ShaderParam10>(vdesc.Name, param));

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

HRESULT DShaderBuffer10::InitPixelShader(ID3DBlob * pShaderBlob, ID3D10Device * pD3DDevice)
{
	// Reflect shader info
	ID3D10ShaderReflection* pPixelShaderReflection = nullptr;

	HRESULT hr = S_OK;

	D3D10_SHADER_DESC shaderDesc;

	D3D10_BUFFER_DESC paramBufferDesc;

	ShaderParam10 param;

	if (FAILED(D3D10ReflectShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &pPixelShaderReflection)))
	{
		return S_FALSE;
	}
	pPixelShaderReflection->GetDesc(&shaderDesc);

	int fsize = sizeof(float);

	int clength = 0;

	unsigned int i;

	for (i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D10ShaderReflectionConstantBuffer* bf = pPixelShaderReflection->GetConstantBufferByIndex(i);
		D3D10_SHADER_BUFFER_DESC desc;
		bf->GetDesc(&desc);

		paramBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		paramBufferDesc.ByteWidth = desc.Size;
		paramBufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
		paramBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		paramBufferDesc.MiscFlags = 0;

		ID3D10Buffer* buffer;

		pD3DDevice->CreateBuffer(&paramBufferDesc, NULL, &buffer);

		clength = desc.Size / fsize;



		int offset = 0;
		int size = 0;

		for (unsigned int j = 0; j < desc.Variables; j++)
		{
			ID3D10ShaderReflectionVariable* bv = bf->GetVariableByIndex(j);

			D3D10_SHADER_VARIABLE_DESC vdesc;
			bv->GetDesc(&vdesc);

			offset = vdesc.StartOffset / fsize;
			size = vdesc.Size / fsize;

			param = ShaderParam10(m_cbufferCount, i, clength, offset, size, 1);

			//param.properties.insert(std::pair<const LPCSTR, ShaderProperty>(vdesc.Name, ShaderProperty(j, size, offset)));

			m_params.insert(std::pair<const LPCSTR, ShaderParam10>(vdesc.Name, param));

			m_propertyCount += 1;
		}


		m_paramBuffers.push_back(buffer);

		m_cbufferCount += 1;
	}

	pPixelShaderReflection->Release();

	return hr;
}
