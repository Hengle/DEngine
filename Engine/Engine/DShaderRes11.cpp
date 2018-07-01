#include "DShaderRes11.h"
#include "DTextureRes11.h"
#include <d3dcompiler.h>

DShaderRes11::DShaderRes11(ID3D11Device * device, ID3D11DeviceContext * deviceContext) : DShaderRes()
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
}

DShaderRes11::~DShaderRes11()
{
}

//void DShaderRes11::GetPropertyInfo(const LPCSTR key, DShaderParamDesc * desc) const
//{
//	if (m_params.find(key) != m_params.end())
//	{
//		DShaderParamDesc pm = m_params.at(key);
//		desc->cbufferIndex = pm.cbufferIndex;
//		desc->cbufferOffset = pm.cbufferOffset;
//		desc->cbufferLength = pm.cbufferLength;
//		desc->propertySize = pm.propertySize;
//		desc->propertyOffset = pm.propertyOffset;
//		desc->shaderType = pm.shaderType;
//		return;
//	}
//	desc->cbufferIndex = -1;
//	desc->cbufferOffset = -1;
//	desc->cbufferLength = 0;
//	desc->propertySize = 0;
//	desc->propertyOffset = -1;
//	desc->shaderType = 0;
//}

//UINT DShaderRes11::GetResOffset(const LPCSTR key) const
//{
//	if (m_resParams.find(key) != m_resParams.end())
//		return m_resParams.at(key);
//	return NAN;
//}

void DShaderRes11::GetResDesc(unsigned int index, DShaderResDesc & res) const
{
	if (index < m_resParams.size())
	{
		res = m_resParams.at(index);
	}
}

bool DShaderRes11::HasProperty(const LPCSTR key) const
{
	size_t size = m_cbuffers.size();
	int i;
	for (i = 0; i < size; i++)
	{
		DShaderCBufferDesc* cdesc = m_cbuffers.at(i);
		if (cdesc == NULL)
			continue;
		if (cdesc->properties.find(key) != cdesc->properties.end())
			return true;
	}
	size = m_resParams.size();
	for (i = 0; i < size; i++)
	{
		DShaderResDesc rdesc = m_resParams.at(i);
		if (rdesc.resName.compare(key) == 0)
			return true;
	}
	/*if (m_cbuffers.find(key) != m_cbuffers.end())
	{
		return true;
	}*/
	/*if (m_resParams.find(key) != m_resParams.end())
		return true;*/
	return false;
}

void DShaderRes11::Release()
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
	if (m_cbuffers.size() > 0)
	{
		for (i = 0; i < m_cbuffers.size(); i++)
		{
			DShaderCBufferDesc* desc = m_cbuffers.at(i);
			delete desc;
		}
	}
	//m_params.clear();
	m_cbuffers.clear();
	m_resParams.clear();

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
	m_deviceContext = NULL;
}

bool DShaderRes11::OnInit(WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;


	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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

	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
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

	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
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

void DShaderRes11::OnDraw()
{
	m_deviceContext->IASetInputLayout(m_layout);

	m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//deviceContext->PSSetSamplers(0, 1, &m_samplerState);

}

void DShaderRes11::OnApplyParams(std::map<std::string, float*>& params, std::map<std::string, float*>&gparams)
{
	int i,j,k;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	float* dataPtr = 0,*paramvalue = 0;
	unsigned int bufferNumber = 0;
	ID3D11Buffer* pbuffer = 0;
	DShaderCBufferDesc* desc = 0;
	std::map<std::string, DShaderPropertyDesc>::iterator iter;

	if (m_cbuffers.size() > 0)
	{
		for (i = 0; i < m_cbuffers.size(); i++)
		{
			desc = m_cbuffers.at(i);
			pbuffer = m_paramBuffers[desc->cbufferIndex];
			if (pbuffer == NULL)
				continue;
			bufferNumber = desc->cbufferStartSlot;
			result = m_deviceContext->Map(pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				continue;
			}

			dataPtr = (float*)mappedResource.pData;

			paramvalue = 0;
			for (iter = desc->properties.begin(); iter != desc->properties.end(); iter++)
			{
				if (params.find(iter->first) != params.end())
				{
					paramvalue = params.at(iter->first);
					for (k = 0; k < iter->second.propertySize; k++)
					{
						dataPtr[k + iter->second.propertyOffset] = paramvalue[k];
					}
				}
				else if (iter->second.isGlobal && gparams.find(iter->first) != gparams.end())
				{
					paramvalue = gparams.at(iter->first);
					for (k = 0; k < iter->second.propertySize; k++)
					{
						dataPtr[k + iter->second.propertyOffset] = paramvalue[k];
					}
				}
			}
			
			m_deviceContext->Unmap(pbuffer, 0);

			if (desc->shaderType == 0)
			{
				m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &pbuffer);
			}
			else
			{
				m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &pbuffer);
			}
		}
	}

	/*std::map<const std::string, UINT>::iterator riter;
	DTexture* tex = 0;
	for (riter = m_resParams.begin(); riter != m_resParams.end(); riter++)
	{
		if (textures.find(riter->first) != textures.end())
		{
			tex = textures.at(riter->first);
			tex->Apply(riter->second);
		}
	}*/
}

//void DShaderRes11::OnApplyParams(int cindex, int coffset, int csize, int stype, float* params)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	float* dataPtr;
//	unsigned int bufferNumber = coffset;
//
//	ID3D11Buffer* pbuffer = m_paramBuffers[cindex];
//
//	result = m_deviceContext->Map(pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		return;
//	}
//
//	dataPtr = (float*)mappedResource.pData;
//
//	int i;
//	for (i = 0; i < csize; i++)
//	{
//		dataPtr[i] = params[i];
//	}
//
//	m_deviceContext->Unmap(pbuffer, 0);
//
//	if (stype == 0)
//	{
//		m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &pbuffer);
//	}
//	else
//	{
//		m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &pbuffer);
//	}
//}

HRESULT DShaderRes11::InitVertexShader(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout, int* inputLayoutByteLength)
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

	//DShaderParamDesc param;
	DShaderCBufferDesc* cbufferdesc = 0;
	DShaderPropertyDesc propertydesc;
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
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
			m_vertexUsage |= 1UL << DVertexUsage_POSITION;
		}
		else if (lstrcmpA(sname, "TEXCOORD") == 0)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			byteOffset += 8;
			if (paramDesc.SemanticIndex == 0)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD0;
			else if (paramDesc.SemanticIndex == 1)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD1;
			else if (paramDesc.SemanticIndex == 2)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD2;
			else if (paramDesc.SemanticIndex == 3)
				m_vertexUsage |= 1UL << DVertexUsage_TEXCOORD3;
		}
		else if (lstrcmpA(sname, "NORMAL") == 0)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
			m_vertexUsage |= 1UL << DVertexUsage_NORMAL;
		}
		else if (lstrcmpA(sname, "COLOR") == 0)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			byteOffset += 16;
			m_vertexUsage |= 1UL << DVertexUsage_COLOR;
		}
		else if (lstrcmpA(sname, "TANGENT") == 0)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			byteOffset += 16;
			m_vertexUsage |= 1UL << DVertexUsage_TANGENT;
		}
		else if (lstrcmpA(sname, "BINORMAL") == 0)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
			m_vertexUsage |= 1UL << DVertexUsage_BINORMAL;
		}
		//// determine DXGI format
		/*else if (paramDesc.Mask == 1)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			byteOffset += 4;
		}
		else if (paramDesc.Mask <= 3)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			byteOffset += 8;
		}
		else if (paramDesc.Mask <= 7)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
		}
		else if (paramDesc.Mask <= 15)
		{
			elementDesc.AlignedByteOffset = byteOffset;
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			byteOffset += 16;
		}*/

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

		cbufferdesc = new DShaderCBufferDesc();
		cbufferdesc->cbufferIndex = m_cbufferCount;
		cbufferdesc->cbufferStartSlot = i;
		cbufferdesc->cbufferSize = clength;
		cbufferdesc->shaderType = 0;

		for (unsigned int j = 0; j < desc.Variables; j++)
		{
			ID3D11ShaderReflectionVariable* bv = bf->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC vdesc;
			bv->GetDesc(&vdesc);

			offset = vdesc.StartOffset / fsize;
			size = vdesc.Size / fsize;

			//param = ShaderParam11(m_cbufferCount, i, clength, offset, size, 0);
			/*param.cbufferIndex = m_cbufferCount;
			param.cbufferOffset = i;
			param.cbufferLength = clength;
			param.propertyOffset = offset;
			param.propertySize = size;
			param.shaderType = 0;
			
			m_params.insert(std::pair<const LPCSTR, DShaderParamDesc>(vdesc.Name, param));*/
			propertydesc.isGlobal = false;
			if (strlen(vdesc.Name) >= 2 && vdesc.Name[0] == 'g' && vdesc.Name[1] == '_')
			{
				propertydesc.isGlobal = true;
			}
			propertydesc.propertyName = vdesc.Name;
			propertydesc.propertyOffset = offset;
			propertydesc.propertySize = size;
			cbufferdesc->properties.insert(std::pair<std::string, DShaderPropertyDesc>(vdesc.Name, propertydesc));

			//param.properties.insert(std::pair<const LPCSTR, ShaderProperty>(vdesc.Name, ShaderProperty(j, size, offset)));

			m_propertyCount += 1;
		}

		m_cbuffers.push_back(cbufferdesc);

		m_paramBuffers.push_back(buffer);

		m_cbufferCount += 1;

		cbufferdesc = 0;
	}

	// Try to create Input Layout
	hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);
	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	//record byte length
	*inputLayoutByteLength = byteOffset;
	return hr;
}

HRESULT DShaderRes11::InitPixelShader(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice)
{
	// Reflect shader info
	ID3D11ShaderReflection* pPixelShaderReflection = nullptr;

	HRESULT hr = S_OK;

	D3D11_SHADER_DESC shaderDesc;

	D3D11_BUFFER_DESC paramBufferDesc;

	//DShaderParamDesc param;
	DShaderCBufferDesc* cbufferdesc = 0;
	DShaderPropertyDesc propertydesc;
	DShaderResDesc resdesc;

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

		cbufferdesc = new DShaderCBufferDesc();
		cbufferdesc->cbufferIndex = m_cbufferCount;
		cbufferdesc->cbufferSize = clength;
		cbufferdesc->cbufferStartSlot = i;
		cbufferdesc->shaderType = 1;

		for (unsigned int j = 0; j < desc.Variables; j++)
		{
			ID3D11ShaderReflectionVariable* bv = bf->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC vdesc;
			bv->GetDesc(&vdesc);

			offset = vdesc.StartOffset / fsize;
			size = vdesc.Size / fsize;

			//param = ShaderParam11(m_cbufferCount, i, clength, offset, size, 1);
			/*param.cbufferIndex = m_cbufferCount;
			param.cbufferOffset = i;
			param.cbufferLength = clength;
			param.propertyOffset = offset;
			param.propertySize = size;
			param.shaderType = 1;*/
			propertydesc.isGlobal = false;
			if (strlen(vdesc.Name) >= 2 && vdesc.Name[0] == 'g' && vdesc.Name[1] == '_')
			{
				propertydesc.isGlobal = true;
			}
			propertydesc.propertyName = vdesc.Name;
			propertydesc.propertyOffset = offset;
			propertydesc.propertySize = size;

			//param.properties.insert(std::pair<const LPCSTR, ShaderProperty>(vdesc.Name, ShaderProperty(j, size, offset)));

			//m_params.insert(std::pair<const LPCSTR, DShaderParamDesc>(vdesc.Name, param));
			cbufferdesc->properties.insert(std::pair<const LPCSTR, DShaderPropertyDesc>(vdesc.Name, propertydesc));

			m_propertyCount += 1;
		}

		m_cbuffers.push_back(cbufferdesc);
		m_paramBuffers.push_back(buffer);

		m_cbufferCount += 1;

		cbufferdesc = 0;
	}

	for (i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC rdesc;
		pPixelShaderReflection->GetResourceBindingDesc(i, &rdesc);

		resdesc.isGlobal = false;
		if (strlen(rdesc.Name) >= 2 && rdesc.Name[0] == 'g' && rdesc.Name[1] == '_')
		{
			resdesc.isGlobal = true;
		}
		resdesc.offset = rdesc.BindPoint;
		resdesc.resName = rdesc.Name;

		//m_resParams.insert(std::pair<std::string, UINT>(rdesc.Name, rdesc.BindPoint));
		m_resParams.push_back(resdesc);

		m_resCount += 1;
	}

	pPixelShaderReflection->Release();

	return hr;
}