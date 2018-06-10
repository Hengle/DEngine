#include "DShaderRes10.h"
#include <D3DX10.h>

DShaderRes10::DShaderRes10(ID3D10Device * device) : DShaderRes()
{
	m_device = device;
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
}

DShaderRes10::~DShaderRes10()
{
}

void DShaderRes10::GetPropertyInfo(const LPCSTR key, DShaderParamDesc * desc) const
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
	}
	desc->cbufferIndex = -1;
	desc->cbufferOffset = -1;
	desc->cbufferLength = 0;
	desc->propertySize = 0;
	desc->propertyOffset = -1;
	desc->shaderType = 0;
}

bool DShaderRes10::HasProperty(const LPCSTR key) const
{
	if (m_params.find(key) != m_params.end())
	{
		return true;
	}
	return false;
}

void DShaderRes10::Release()
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
	m_device = NULL;
}

HRESULT DShaderRes10::InitVertexShader(ID3DBlob * pShaderBlob, ID3D10Device *pD3DDevice, ID3D10InputLayout **pInputLayout, int * inputLayoutByteLength)
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

	DShaderParamDesc param;

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

			//param = ShaderParam10(m_cbufferCount, i, clength, offset, size, 0);
			param.cbufferIndex = m_cbufferCount;
			param.cbufferOffset = i;
			param.cbufferLength = clength;
			param.propertyOffset = offset;
			param.propertySize = size;
			param.shaderType = 0;
			m_params.insert(std::pair<const LPCSTR, DShaderParamDesc>(vdesc.Name, param));

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

HRESULT DShaderRes10::InitPixelShader(ID3DBlob * pShaderBlob, ID3D10Device * pD3DDevice)
{
	// Reflect shader info
	ID3D10ShaderReflection* pPixelShaderReflection = nullptr;

	HRESULT hr = S_OK;

	D3D10_SHADER_DESC shaderDesc;

	D3D10_BUFFER_DESC paramBufferDesc;

	DShaderParamDesc param;

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

			//param = ShaderParam10(m_cbufferCount, i, clength, offset, size, 1);
			param.cbufferIndex = m_cbufferCount;
			param.cbufferOffset = i;
			param.cbufferLength = clength;
			param.propertyOffset = offset;
			param.propertySize = size;
			param.shaderType = 1;

			//param.properties.insert(std::pair<const LPCSTR, ShaderProperty>(vdesc.Name, ShaderProperty(j, size, offset)));

			m_params.insert(std::pair<const LPCSTR, DShaderParamDesc>(vdesc.Name, param));

			m_propertyCount += 1;
		}


		m_paramBuffers.push_back(buffer);

		m_cbufferCount += 1;
	}

	pPixelShaderReflection->Release();

	return hr;
}

bool DShaderRes10::OnInit(WCHAR * vsFilename, WCHAR * psFilename)
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

	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout.

	int byteLength = 0;
	m_cbufferCount = 0;
	m_propertyCount = 0;

	result = InitVertexShader(vertexShaderBuffer, m_device, &m_layout, &byteLength);
	if (FAILED(result))
	{
		return false;
	}

	result = InitPixelShader(pixelShaderBuffer, m_device);
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

void DShaderRes10::OnDraw()
{
	m_device->IASetInputLayout(m_layout);

	m_device->VSSetShader(m_vertexShader);
	m_device->PSSetShader(m_pixelShader);

	//deviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void DShaderRes10::OnApplyParams(int cindex, int coffset, int csize, int stype, float* params)
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
		m_device->VSSetConstantBuffers(bufferNumber, 1, &pbuffer);
	}
	else
	{
		m_device->PSSetConstantBuffers(bufferNumber, 1, &pbuffer);
	}
}
