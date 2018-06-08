#include "D3D9Core.h"
#include <fstream>

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

void D3D9Core::BeginRender(float r, float g, float b, float a)
{
	m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(r, g, b, a), 1.0f, 0);

	m_device->BeginScene();

	m_device->SetViewport(&m_viewPort);
}

void D3D9Core::EndRender()
{
	m_device->EndScene();

	m_device->Present(0, 0, 0, 0);

	//m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

DMeshBuffer * D3D9Core::CreateMeshBuffer(int vertexCount, int indexCount, int bufferLength, int dataSize, const float * vertices, const unsigned long * indices)
{
	DMeshBuffer9* buffer = new DMeshBuffer9();
	buffer->Init(m_device, vertexCount, indexCount, bufferLength, dataSize, vertices, indices);
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
	DShaderBuffer9* bf9 = (DShaderBuffer9*)shaderBuffer;
	bf9->ApplyBuffer(m_device, cindex, csize, stype, params);
}

void D3D9Core::ApplyTextureParams(DTextureBuffer * textureBuffer)
{
}

void D3D9Core::DrawMesh(const DMeshBuffer * buffer, int)
{
	DMeshBuffer9* bf9 = (DMeshBuffer9*)buffer;
	bf9->Draw(m_device);
}

void D3D9Core::DrawShader(const DShaderBuffer * buffer, int)
{
	((DShaderBuffer9*)buffer)->Draw(m_device);
}

void D3D9Core::SetBackBufferRenderTarget()
{
}

DMeshBuffer9::DMeshBuffer9()
{
	m_mesh = 0;
	/*m_vertexBuffer = 0;
	m_indexBuffer = 0;*/
}

void DMeshBuffer9::Init(LPDIRECT3DDEVICE9 device, int vertexCount, int indexCount, int bufferLength, int dataSize, const float* vertices, const unsigned long* indices)
{

	//device->CreateVertexBuffer(dataSize*vertexCount, D3DUSAGE_WRITEONLY, D3DFVF_XYZ, D3DPOOL_MANAGED, &m_vertexBuffer, 0);
	//device->CreateIndexBuffer(sizeof(unsigned long)*indexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuffer, 0);
	D3DVERTEXELEMENT9 elements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};

	m_dataSize = dataSize;
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
	m_pimCount = indexCount / 3;
	int vlength = vertexCount*bufferLength;

	D3DXCreateMesh(m_pimCount, vertexCount, D3DXMESH_MANAGED, elements, device, &m_mesh);
	//m_vertexBuffer->Lock()
	float* ves;
	int i;
	m_mesh->LockVertexBuffer(0, (void**)&ves);

	for (i = 0; i < vlength; i++)
	{
		ves[i] = vertices[i];
	}

	m_mesh->UnlockVertexBuffer();

	WORD* ids = 0;
	m_mesh->LockIndexBuffer(0, (void**)&ids);

	for (i = 0; i < indexCount; i++)
	{
		ids[i] = indices[i];
	}

	m_mesh->UnlockIndexBuffer();
}

void DMeshBuffer9::Draw(LPDIRECT3DDEVICE9 device)
{
	//device->SetStreamSource(0, m_vertexBuffer, 0, m_dataSize);
	//device->SetIndices(m_indexBuffer);
	//device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	//device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_pimCount);
	m_mesh->DrawSubset(0);
}

void DMeshBuffer9::Release()
{
	if (m_mesh != NULL)
	{
		m_mesh->Release();
		m_mesh = NULL;
	}
	/*if (m_vertexBuffer != NULL)
		m_vertexBuffer->Release();
	m_vertexBuffer = NULL;
	if (m_indexBuffer != NULL)
		m_indexBuffer->Release();
	m_indexBuffer = NULL;*/
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
	m_propertyCount = 0;
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
	return m_propertyCount;
}

unsigned int DShaderBuffer9::GetPropertyCount() const
{
	return m_propertyCount;
}

void DShaderBuffer9::GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int & poffset, int & plength, int & stype) const
{
	if (m_params.find(key) != m_params.end())
	{
		ShaderParam9 pm = m_params.at(key);
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

bool DShaderBuffer9::HasProperty(const LPCSTR key) const
{
	if (m_params.find(key) != m_params.end())
	{
		return true;
	}
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

void DShaderBuffer9::SetMatrix(const LPCSTR key, LPDIRECT3DDEVICE9 device, DMatrix4x4 matrix)
{
	if (m_params.find(key) != m_params.end())
	{
		ShaderParam9 param = m_params.at(key);
		D3DXHANDLE handle = m_handles.at(param.bufferIndex);
		/*D3DXMATRIX m = D3DXMATRIX(matrix.m00, matrix.m01, matrix.m02, matrix.m03,
			matrix.m10, matrix.m11, matrix.m12, matrix.m13,
			matrix.m20, matrix.m21, matrix.m22, matrix.m23,
			matrix.m30, matrix.m31, matrix.m32, matrix.m33);*/
		float v[16];
		v[0] = matrix.m00;
		v[1] = matrix.m01;
		v[2] = matrix.m02;
		v[3] = matrix.m03;
		v[4] = matrix.m10;
		v[5] = matrix.m11;
		v[6] = matrix.m12;
		v[7] = matrix.m13;
		v[8] = matrix.m20;
		v[9] = matrix.m21;
		v[10] = matrix.m22;
		v[11] = matrix.m23;
		v[12] = matrix.m30;
		v[13] = matrix.m31;
		v[14] = matrix.m32;
		v[15] = matrix.m33;
		m_vertexConstable->SetValue(device, handle, v, 64);
	}
	return;
}

bool DShaderBuffer9::InitShader(LPDIRECT3DDEVICE9 device, WCHAR* vsfile, WCHAR* psfile)
{
	HRESULT hr;
	ID3DXBuffer* vshader,*pshader = 0;
	ID3DXBuffer* errorBuffer = 0;
	hr = D3DXCompileShaderFromFile(
		vsfile,
		0,
		0,
		"Main", // entry point function name
		"vs_2_0",
		D3DXSHADER_DEBUG,
		&vshader,
		&errorBuffer,
		&m_vertexConstable);

	if (errorBuffer)
	{
		char* log = (char*)errorBuffer->GetBufferPointer();
		errorBuffer->Release();
		errorBuffer = 0;
	}

	if (FAILED(hr))
	{
		return false;
	}

	hr = D3DXCompileShaderFromFile(
		psfile,
		0,
		0,
		"Main",
		"ps_2_0",
		D3DXSHADER_DEBUG,
		&pshader,
		&errorBuffer,
		&m_pixelConstable);

	if (errorBuffer)
	{
		char* compileErrors;
		unsigned long bufferSize, i;
		std::ofstream fout;


		// Get a pointer to the error message text buffer.
		compileErrors = (char*)(errorBuffer->GetBufferPointer());

		// Get the length of the message.
		bufferSize = errorBuffer->GetBufferSize();

		// Open a file to write the error message to.
		fout.open("shader-error.txt");

		// Write out the error message.
		for (i = 0; i<bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		// Close the file.
		fout.close();

		// Release the error message.
		errorBuffer->Release();
		errorBuffer = 0;
		//char* log = (char*)errorBuffer->GetBufferPointer();
	}

	if (FAILED(hr))
	{
		return false;
	}

	

	hr = device->CreateVertexShader((DWORD*)vshader->GetBufferPointer(), &m_vertexShader);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreatePixelShader((DWORD*)pshader->GetBufferPointer(), &m_pixelShader);
	if (FAILED(hr))
	{
		return false;
	}

	hr = InitVertexShader();
	if (FAILED(hr))
		return false;

	hr = InitPixelShader();
	if (FAILED(hr))
		return false;

	if (vshader != NULL)
		vshader->Release();
	vshader = 0;
	if (pshader != NULL)
		pshader->Release();
	pshader = 0;
	return true;
}

HRESULT DShaderBuffer9::InitVertexShader()
{
	D3DXCONSTANTTABLE_DESC desc;
	D3DXCONSTANT_DESC cdesc;
	D3DXHANDLE handle;
	HRESULT result;

	result = m_vertexConstable->GetDesc(&desc);

	if (FAILED(result))
	{
		return S_FALSE;
	}

	unsigned int i;
	unsigned int pcount;
	int fsize = sizeof(float);
	int length = 0;
	for (i = 0; i < desc.Constants; i++)
	{
		handle = m_vertexConstable->GetConstant(0, i);
		m_vertexConstable->GetConstantDesc(handle, &cdesc, &pcount);

		length = cdesc.Bytes / fsize;

		ShaderParam9 param = ShaderParam9(m_propertyCount, m_propertyCount, cdesc.Bytes, 0, length, 0);

		m_params.insert(std::pair<std::string, ShaderParam9>(cdesc.Name, param));
		m_handles.push_back(handle);

		m_propertyCount += 1;
	}

	return S_OK;
}

HRESULT DShaderBuffer9::InitPixelShader()
{
	D3DXCONSTANTTABLE_DESC desc;
	D3DXCONSTANT_DESC cdesc;
	D3DXHANDLE handle;
	HRESULT result;

	result = m_pixelConstable->GetDesc(&desc);

	if (FAILED(result))
	{
		return S_FALSE;
	}

	unsigned int i;
	unsigned int pcount;
	int fsize = sizeof(float);
	int length = 0;
	for (i = 0; i < desc.Constants; i++)
	{
		handle = m_pixelConstable->GetConstant(0, i);
		m_pixelConstable->GetConstantDesc(handle, &cdesc, &pcount);

		length = cdesc.Bytes / fsize;

		ShaderParam9 param = ShaderParam9(m_propertyCount, m_propertyCount, cdesc.Bytes, 0, length, 1);

		m_params.insert(std::pair<std::string, ShaderParam9>(cdesc.Name, param));
		m_handles.push_back(handle);


		m_propertyCount += 1;
	}

	return S_OK;
}

void DShaderBuffer9::ApplyBuffer(LPDIRECT3DDEVICE9 device, int cindex, int csize, int stype, float* params)
{
	if (cindex < 0 || cindex >= m_handles.size())
		return;
	D3DXHANDLE handle = m_handles.at(cindex);
	if (stype == 0)
		m_vertexConstable->SetValue(device, handle, params, csize);
	else
		m_pixelConstable->SetValue(device, handle, params, csize);
}

void DShaderBuffer9::Draw(LPDIRECT3DDEVICE9 device)
{
	device->SetVertexShader(m_vertexShader);
	device->SetPixelShader(m_pixelShader);
}