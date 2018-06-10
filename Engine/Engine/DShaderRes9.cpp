#include "DShaderRes9.h"

DShaderRes9::DShaderRes9(LPDIRECT3DDEVICE9 device) : DShaderRes()
{
	m_device = device;
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_vertexConstable = 0;
	m_pixelConstable = 0;
}

DShaderRes9::~DShaderRes9()
{
}

void DShaderRes9::GetPropertyInfo(const LPCSTR key, DShaderParamDesc * desc) const
{
	if (m_params.find(key) != m_params.end())
	{
		DShaderParamDesc pm = m_params.at(key);
		desc->cbufferIndex = pm.cbufferIndex;
		desc->cbufferOffset = pm.cbufferOffset;
		desc->cbufferLength = pm.cbufferLength;
		desc->propertySize = pm.propertySize;
		desc->propertyOffset = pm.propertyOffset;
		desc->shaderType = pm.shaderType;
		return;
		//}
	}
	desc->cbufferIndex = -1;
	desc->cbufferOffset = -1;
	desc->cbufferLength = 0;
	desc->propertySize =  0;
	desc->propertyOffset = -1;
	desc->shaderType = 0;
}

bool DShaderRes9::HasProperty(const LPCSTR key) const
{
	if (m_params.find(key) != m_params.end())
	{
		return true;
	}
	return false;
}

void DShaderRes9::Release()
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

bool DShaderRes9::OnInit(WCHAR * vsfile, WCHAR * psfile)
{
	HRESULT hr;
	ID3DXBuffer* vshader, *pshader = 0;
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
		char* log = (char*)errorBuffer->GetBufferPointer();
		errorBuffer->Release();
		errorBuffer = 0;
	}

	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->CreateVertexShader((DWORD*)vshader->GetBufferPointer(), &m_vertexShader);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->CreatePixelShader((DWORD*)pshader->GetBufferPointer(), &m_pixelShader);
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

	m_cbufferCount = m_propertyCount;
	return true;
}

void DShaderRes9::OnDraw()
{
	m_device->SetVertexShader(m_vertexShader);
	m_device->SetPixelShader(m_pixelShader);
}

void DShaderRes9::OnApplyParams(int cindex, int coffset, int csize, int stype, float* params)
{
	if (cindex < 0 || cindex >= m_handles.size())
		return;
	D3DXHANDLE handle = m_handles.at(cindex);
	if (stype == 0)
		m_vertexConstable->SetValue(m_device, handle, params, csize);
	else
		m_pixelConstable->SetValue(m_device, handle, params, csize);
}

HRESULT DShaderRes9::InitVertexShader()
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

		DShaderParamDesc param;
		param.cbufferIndex = m_propertyCount;
		param.cbufferOffset = m_propertyCount;
		param.cbufferLength = cdesc.Bytes;
		param.propertyOffset = 0;
		param.propertySize = length;
		param.shaderType = 0;

		m_params.insert(std::pair<std::string, DShaderParamDesc>(cdesc.Name, param));
		m_handles.push_back(handle);

		m_propertyCount += 1;
	}

	return S_OK;
}

HRESULT DShaderRes9::InitPixelShader()
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

		DShaderParamDesc param;
		param.cbufferIndex = m_propertyCount;
		param.cbufferOffset = m_propertyCount;
		param.cbufferLength = cdesc.Bytes;
		param.propertyOffset = 0;
		param.propertySize = length;
		param.shaderType = 1;

		m_params.insert(std::pair<std::string, DShaderParamDesc>(cdesc.Name, param));
		m_handles.push_back(handle);


		m_propertyCount += 1;
	}

	return S_OK;
}
