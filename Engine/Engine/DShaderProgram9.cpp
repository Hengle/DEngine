#include "DShaderProgram9.h"
#include "DLog.h"

DShaderProgram9::DShaderProgram9(LPDIRECT3DDEVICE9 device) : DShaderProgram()
{
	m_device = device;
}

DShaderProgram9::~DShaderProgram9()
{
}

void DShaderProgram9::GetResDesc(unsigned int index, DShaderResDesc & res) const
{
	if (index < m_resParams.size())
	{
		res = m_resParams.at(index);
	}
}

bool DShaderProgram9::HasProperty(const LPCSTR key) const
{
	size_t size = m_properties.size();
	int i;
	for (i = 0; i < size; i++)
	{
		DShaderPropertyDesc9 pdesc = m_properties.at(i);
		if (pdesc.propertyName.compare(key) == 0)
			return true;
	}
	size = m_resParams.size();
	for (i = 0; i < size; i++)
	{
		DShaderResDesc rdesc = m_resParams.at(i);
		if (rdesc.resName.compare(key) == 0)
			return true;
	}
	/*if (m_params.find(key) != m_params.end())
	{
		return true;
	}
	if (m_resParams.find(key) != m_resParams.end())
		return true;*/
	return false;
}

void DShaderProgram9::Release()
{
	m_resParams.clear();
	m_handles.clear();
	m_properties.clear();
}

void DShaderProgram9::OnApplyParams(std::map<std::string, float*>& params, std::map<std::string, float*>&gparams)
{
	int i;
	size_t size = m_properties.size();
	D3DXHANDLE handle;
	float* value;
	for (i = 0; i < size; i++)
	{
		DShaderPropertyDesc9 pdesc = m_properties.at(i);
		if (params.find(pdesc.propertyName) != params.end())
		{
			handle = m_handles.at(pdesc.propertyOffset);
			value = params.at(pdesc.propertyName);
			OnSetValue(handle, value, pdesc.propertySize);
			/*if(pdesc.shaderType == 0)
				m_vertexConstable->SetValue(m_device, handle, value, pdesc.propertySize);
			else
				m_pixelConstable->SetValue(m_device, handle, value, pdesc.propertySize);*/
		}
		else if (pdesc.isGlobal && gparams.find(pdesc.propertyName) != gparams.end())
		{
			handle = m_handles.at(pdesc.propertyOffset);
			value = gparams.at(pdesc.propertyName);
			OnSetValue(handle, value, pdesc.propertySize);
			/*if (pdesc.shaderType == 0)
				m_vertexConstable->SetValue(m_device, handle, value, pdesc.propertySize);
			else
				m_pixelConstable->SetValue(m_device, handle, value, pdesc.propertySize);*/
		}
	}
}

void DShaderProgram9::OutputErrorLog(ID3DXBuffer * errorBuffer)
{
	char* log = (char*)errorBuffer->GetBufferPointer();
	DLog::Err("Shader Compile Error:");
	DLog::Err(log);
}

DShaderVertexProgram9::DShaderVertexProgram9(LPDIRECT3DDEVICE9 device) : DShaderProgram9(device)
{
	m_vertexShader = 0;
	m_vertexConstable = 0;
}

DShaderVertexProgram9::~DShaderVertexProgram9()
{
}

void DShaderVertexProgram9::Release()
{
	DShaderProgram9::Release();
	if (m_vertexShader != NULL)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}
	if (m_vertexConstable != NULL)
	{
		m_vertexConstable->Release();
		m_vertexConstable = NULL;
	}
}

bool DShaderVertexProgram9::OnInit(const char * content, char * funcName)
{
	HRESULT hr;
	ID3DXBuffer* vshader = 0;
	ID3DXBuffer* errorBuffer = 0;

	UINT len = strlen(content);
	hr = D3DXCompileShader(content, len, 0, 0, funcName, "vs_2_0", D3DXSHADER_DEBUG, &vshader, &errorBuffer, &m_vertexConstable);

	if (errorBuffer)
	{
		OutputErrorLog(errorBuffer);
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

	hr = InitVertexShader(vshader);
	if (FAILED(hr)) {
		return false;
	}

	if (vshader != NULL)
		vshader->Release();
	vshader = 0;
	return true;
}

void DShaderVertexProgram9::OnDraw()
{
	m_device->SetVertexShader(m_vertexShader);
}

void DShaderVertexProgram9::OnSetValue(D3DXHANDLE & handle, float * value, UINT bytes)
{
	m_vertexConstable->SetValue(m_device, handle, value, bytes);
}

HRESULT DShaderVertexProgram9::InitVertexShader(ID3DXBuffer * vertexShaderBuffer)
{
	D3DXCONSTANTTABLE_DESC desc;
	D3DXCONSTANT_DESC cdesc;
	D3DXHANDLE handle;
	HRESULT result;
	DShaderPropertyDesc9 pdesc;

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

		/*DShaderParamDesc param;
		param.cbufferIndex = m_propertyCount;
		param.cbufferOffset = m_propertyCount;
		param.cbufferLength = cdesc.Bytes;
		param.propertyOffset = 0;
		param.propertySize = length;
		param.shaderType = 0;*/
		pdesc.isGlobal = false;
		if (strlen(cdesc.Name) >= 2 && cdesc.Name[0] == 'g' && cdesc.Name[1] == '_')
		{
			pdesc.isGlobal = true;
		}
		pdesc.propertyName = cdesc.Name;
		pdesc.propertySize = cdesc.Bytes;
		pdesc.propertyOffset = m_cbufferCount;

		//m_params.insert(std::pair<std::string, DShaderParamDesc>(cdesc.Name, param));
		m_properties.push_back(pdesc);
		m_handles.push_back(handle);

		m_propertyCount += 1;
		m_cbufferCount += 1;
	}

	m_vertexConstable->SetDefaults(m_device);

	DWORD* vfunc = (DWORD*)vertexShaderBuffer->GetBufferPointer();
	UINT scount = 0;

	result = D3DXGetShaderInputSemantics(vfunc, NULL, &scount);

	if (FAILED(result))
		return S_FALSE;

	D3DXSEMANTIC* semantics = new D3DXSEMANTIC[scount];

	result = D3DXGetShaderInputSemantics(vfunc, semantics, &scount);

	if (FAILED(result))
	{
		delete[] semantics;
		return S_FALSE;
	}

	for (i = 0; i < scount; i++)
	{
		D3DXSEMANTIC d = semantics[i];
		switch (d.Usage)
		{
		case D3DDECLUSAGE_POSITION:
			m_vertexUsage |= 1UL << DVertexUsage_POSITION;
			break;
		case D3DDECLUSAGE_TEXCOORD:
			if (d.UsageIndex == 0)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD0;
			else if (d.UsageIndex == 1)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD1;
			else if (d.UsageIndex == 2)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD2;
			else if (d.UsageIndex == 3)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD3;
			break;
		case D3DDECLUSAGE_COLOR:
			m_vertexUsage |= 1UL << DVertexUsage_COLOR;
			break;
		case D3DDECLUSAGE_NORMAL:
			m_vertexUsage |= 1UL << DVertexUsage_NORMAL;
			break;
		case D3DDECLUSAGE_TANGENT:
			m_vertexUsage |= 1UL << DVertexUsage_TANGENT;
			break;
		case D3DDECLUSAGE_BINORMAL:
			m_vertexUsage |= 1UL << DVertexUsage_BINORMAL;
			break;
		default:
			break;
		}
	}

	delete[] semantics;

	return S_OK;
}

DShaderPixelProgram9::DShaderPixelProgram9(LPDIRECT3DDEVICE9 device) : DShaderProgram9(device)
{
	m_pixelShader = 0;
	m_pixelConstable = 0;
}

DShaderPixelProgram9::~DShaderPixelProgram9()
{
}

void DShaderPixelProgram9::Release()
{
	DShaderProgram9::Release();
	if (m_pixelShader != NULL)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}
	if (m_pixelConstable != NULL)
	{
		m_pixelConstable->Release();
		m_pixelConstable = NULL;
	}
}

bool DShaderPixelProgram9::OnInit(const char * content, char * funcName)
{
	HRESULT hr;
	ID3DXBuffer *pshader = 0;
	ID3DXBuffer* errorBuffer = 0;

	UINT len = strlen(content);

	hr = D3DXCompileShader(content, len, 0, 0, funcName, "ps_2_0", D3DXSHADER_DEBUG, &pshader, &errorBuffer, &m_pixelConstable);

	if (errorBuffer)
	{
		OutputErrorLog(errorBuffer);
		errorBuffer->Release();
		errorBuffer = 0;
	}

	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->CreatePixelShader((DWORD*)pshader->GetBufferPointer(), &m_pixelShader);
	if (FAILED(hr))
	{
		return false;
	}

	hr = InitPixelShader();
	if (FAILED(hr))
		return false;

	if (pshader != NULL)
		pshader->Release();
	pshader = 0;
	return true;
}

void DShaderPixelProgram9::OnDraw()
{
	m_device->SetPixelShader(m_pixelShader);
}

void DShaderPixelProgram9::OnSetValue(D3DXHANDLE & handle, float * value, UINT bytes)
{
	m_pixelConstable->SetValue(m_device, handle, value, bytes);
}

HRESULT DShaderPixelProgram9::InitPixelShader()
{
	D3DXCONSTANTTABLE_DESC desc;
	D3DXCONSTANT_DESC cdesc;
	D3DXHANDLE handle;
	HRESULT result;
	DShaderPropertyDesc9 pdesc;
	DShaderResDesc rdesc;

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

		if (cdesc.Class == D3DXPC_OBJECT)
		{
			rdesc.isGlobal = false;
			if (strlen(cdesc.Name) >= 2 && cdesc.Name[0] == 'g' && cdesc.Name[1] == '_')
			{
				rdesc.isGlobal = true;
			}
			rdesc.resName = cdesc.Name;
			rdesc.offset = cdesc.RegisterIndex;
			//m_resParams.insert(std::pair<std::string, D3DXCONSTANT_DESC>(cdesc.Name, cdesc));
			m_resParams.push_back(rdesc);
			m_resCount += 1;
			m_propertyCount += 1;
		}
		else
		{

			/*DShaderParamDesc param;
			param.cbufferIndex = m_propertyCount;
			param.cbufferOffset = m_propertyCount;
			param.cbufferLength = cdesc.Bytes;
			param.propertyOffset = cdesc.RegisterIndex;
			param.propertySize = length;
			param.shaderType = 1;

			m_params.insert(std::pair<std::string, DShaderParamDesc>(cdesc.Name, param));*/

			pdesc.isGlobal = false;
			if (strlen(cdesc.Name) >= 2 && cdesc.Name[0] == 'g' && cdesc.Name[1] == '_')
			{
				pdesc.isGlobal = true;
			}
			pdesc.propertyName = cdesc.Name;
			pdesc.propertySize = cdesc.Bytes;
			pdesc.propertyOffset = m_cbufferCount;
			m_properties.push_back(pdesc);

			m_handles.push_back(handle);

			m_propertyCount += 1;
			m_cbufferCount += 1;
		}
	}

	m_pixelConstable->SetDefaults(m_device);

	return S_OK;
}
