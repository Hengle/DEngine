#include "stdafx.h"
#include "MyTestShader.h"


MyTestShader::MyTestShader()
{
	m_vertexShader = 0;
	//m_pixelShader = 0;
	m_vertexConstable = 0;
	//m_pixelConstable = 0;
	//m_propertyCount = 0;
}


MyTestShader::~MyTestShader()
{
}

void MyTestShader::Init(LPDIRECT3DDEVICE9 device, WCHAR * vsfile, WCHAR* psfile)
{
	HRESULT hr;
	ID3DXBuffer* vshader,*pshader= 0;
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
		return;
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
		char* log = (char*)errorBuffer->GetBufferPointer();
		errorBuffer->Release();
		errorBuffer = 0;
	}

	if (FAILED(hr))
	{
		return;
	}

	hr = device->CreateVertexShader((DWORD*)vshader->GetBufferPointer(), &m_vertexShader);
	if (FAILED(hr))
	{
		return;
	}

	hr = device->CreatePixelShader((DWORD*)pshader->GetBufferPointer(), &m_pixelShader);
	if (FAILED(hr))
	{
		return;
	}

	/*hr = InitVertexShader();
	if (FAILED(hr))
		return;*/

	if (vshader != NULL)
		vshader->Release();
	vshader = 0;

	if (pshader != NULL)
		pshader->Release();
	pshader = 0;

	m_worldMatrix = m_vertexConstable->GetConstantByName(0, "worldMatrix");
	m_viewMatrix = m_vertexConstable->GetConstantByName(0, "viewMatrix");
	m_projMatrix = m_vertexConstable->GetConstantByName(0, "projectionMatrix");
	
	return;
}

void MyTestShader::Release()
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

void MyTestShader::SetWorldMatrix(LPDIRECT3DDEVICE9 device, D3DXMATRIX* matrix)
{
	m_vertexConstable->SetMatrix(device, m_worldMatrix, matrix);
}

void MyTestShader::SetViewMatrix(LPDIRECT3DDEVICE9 device, D3DXMATRIX* matrix)
{
	m_vertexConstable->SetMatrix(device, m_viewMatrix, matrix);
}

void MyTestShader::SetProjMatrix(LPDIRECT3DDEVICE9 device, D3DXMATRIX* matrix)
{
	m_vertexConstable->SetMatrix(device, m_projMatrix, matrix);
}

void MyTestShader::Draw(LPDIRECT3DDEVICE9 device)
{
	device->SetVertexShader(m_vertexShader);
	device->SetPixelShader(m_pixelShader);
}

//HRESULT MyTestShader::InitVertexShader()
//{
//	D3DXCONSTANTTABLE_DESC desc;
//	D3DXCONSTANT_DESC cdesc;
//	D3DXHANDLE handle;
//	HRESULT result;
//
//	result = m_vertexConstable->GetDesc(&desc);
//
//	if (FAILED(result))
//	{
//		return S_FALSE;
//	}
//
//	unsigned int i;
//	unsigned int pcount;
//	int fsize = sizeof(float);
//	int length = 0;
//	for (i = 0; i < desc.Constants; i++)
//	{
//		handle = m_vertexConstable->GetConstant(0, i);
//		m_vertexConstable->GetConstantDesc(handle, &cdesc, &pcount);
//
//		length = cdesc.Bytes / fsize;
//
//		ShaderParam9 param = ShaderParam9(m_propertyCount, m_propertyCount, cdesc.Bytes, m_propertyCount, length, 0);
//
//		m_params.insert(std::pair<std::string, ShaderParam9>(cdesc.Name, param));
//		m_handles.push_back(handle);
//
//		m_propertyCount += 1;
//	}
//
//	return S_OK;
//}
