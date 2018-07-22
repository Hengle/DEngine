#ifdef _DGAPI_D3D11
#include "DRenderStateMgr11.h"

DRenderStateMgr11::DRenderStateMgr11(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_disableBlendState = 0;
}

DRenderStateMgr11::~DRenderStateMgr11()
{
}

void DRenderStateMgr11::Init()
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	//InitRasterizerStates();
	m_currentRasterizerState.fillMode = DFillMode_Solid;
	m_currentRasterizerState.cullMode = DCullMode_Back;

	RefreshRasterizerState();
	//ChangeCullMode(DCullMode_Back);

	m_currentDepthStencilState.ztest = DRSCompareFunc_LEqual;
	m_currentDepthStencilState.zwrite = true;
	m_currentDepthStencilState.stencilId = 0;
	m_currentDepthStencilState.enableStencil = false;

	RefreshDepthStencilState();

	
	m_currentBlendState.enableBlend = false;
	RefreshBlendStencilState();
}

void DRenderStateMgr11::Release()
{
	m_device = NULL;
	m_deviceContext = NULL;
	std::map<unsigned int, ID3D11RasterizerState*>::iterator  riter;
	for (riter = m_rasterizerStates.begin(); riter != m_rasterizerStates.end(); riter++)
	{
		if (riter->second != NULL)
		{
			riter->second->Release();
		}
	}
	m_rasterizerStates.clear();

	std::map<unsigned long, ID3D11DepthStencilState*>::iterator  diter;
	for (diter = m_depthStencilStates.begin(); diter != m_depthStencilStates.end(); diter++)
	{
		if (diter->second != NULL)
		{
			diter->second->Release();
		}
	}
	m_depthStencilStates.clear();

	std::map<unsigned long, ID3D11BlendState*>::iterator  biter;
	for (biter = m_blendStates.begin(); biter != m_blendStates.end(); biter++)
	{
		if (biter->second != NULL)
		{
			biter->second->Release();
		}
	}
	m_blendStates.clear();

	if (m_disableBlendState != NULL)
	{
		m_disableBlendState->Release();
		m_disableBlendState = NULL;
	}
}

void DRenderStateMgr11::SetCullMode(DCullMode cullMode)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentRasterizerState.cullMode == cullMode)
		return;
	m_currentRasterizerState.cullMode = cullMode;
	//if (m_currentMode == cullMode)
	//	return;
	//ChangeCullMode(cullMode);
	RefreshRasterizerState();
}

void DRenderStateMgr11::SetFillMode(DFillMode fillMode)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentRasterizerState.fillMode == fillMode)
		return;
	m_currentRasterizerState.fillMode = fillMode;
	RefreshRasterizerState();
}

void DRenderStateMgr11::SetZWriteEnable(bool zwrite)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.zwrite == zwrite)
		return;
	m_currentDepthStencilState.zwrite = zwrite;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetZTestFunc(DRSCompareFunc ztest)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.ztest == ztest)
		return;
	m_currentDepthStencilState.ztest = ztest;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetBlendOp(DRSBlendOp op)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentBlendState.blendOp == op)
		return;
	m_currentBlendState.blendOp = op;
	if (m_currentBlendState.enableBlend == false)
		return;
	RefreshBlendStencilState();
}

void DRenderStateMgr11::SetBlendEnable(bool enableBlend)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentBlendState.enableBlend == enableBlend)
		return;
	m_currentBlendState.enableBlend = enableBlend;
	RefreshBlendStencilState();
}

void DRenderStateMgr11::SetBlendSrcFactor(DRSBlendFactor factor)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentBlendState.srcfactor == factor)
		return;
	m_currentBlendState.srcfactor = factor;
	if (m_currentBlendState.enableBlend == false)
		return;
	RefreshBlendStencilState();
}

void DRenderStateMgr11::SetBlendDstFactor(DRSBlendFactor factor)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentBlendState.dstfactor == factor)
		return;
	m_currentBlendState.dstfactor = factor;
	if (m_currentBlendState.enableBlend == false)
		return;
	RefreshBlendStencilState();
}

void DRenderStateMgr11::SetStencilRefId(UINT stencilId)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.stencilId == stencilId)
		return;
	m_currentDepthStencilState.stencilId = stencilId;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetStencilEnable(bool enableStencil)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.enableStencil == enableStencil)
		return;
	m_currentDepthStencilState.enableStencil = enableStencil;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetStencilReadMask(unsigned short readMask)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.stencilReadMask == readMask)
		return;
	m_currentDepthStencilState.stencilReadMask = readMask;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetStencilWriteMask(unsigned short writeMask)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.stencilWriteMask == writeMask)
		return;
	m_currentDepthStencilState.stencilWriteMask = writeMask;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetStencilComparisonFunc(DRSCompareFunc compFunc)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.stencilComp == compFunc)
		return;
	m_currentDepthStencilState.stencilComp = compFunc;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetStencilPassOp(DRSStencilOp stencilPass)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.stencilPassOp == stencilPass)
		return;
	m_currentDepthStencilState.stencilPassOp = stencilPass;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetStencilFailOp(DRSStencilOp stencilFail)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.stencilFailOp == stencilFail)
		return;
	m_currentDepthStencilState.stencilFailOp = stencilFail;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::SetStencilZFailOp(DRSStencilOp stencilZFail)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (m_currentDepthStencilState.stencilZFailOp == stencilZFail)
		return;
	m_currentDepthStencilState.stencilZFailOp = stencilZFail;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr11::RefreshDepthStencilState()
{
	unsigned long key = m_currentDepthStencilState.GetKey();
	if (m_depthStencilStates.find(key) != m_depthStencilStates.end())
	{
		ID3D11DepthStencilState* state = m_depthStencilStates.at(key);
		m_deviceContext->OMSetDepthStencilState(state, m_currentDepthStencilState.stencilId);
	}
	else
	{
		ID3D11DepthStencilState* state;
		HRESULT result = CreateDepthStencilState(m_currentDepthStencilState, &state);
		if (!FAILED(result))
		{
			m_depthStencilStates.insert(std::pair<unsigned long, ID3D11DepthStencilState*>(key, state));
			m_deviceContext->OMSetDepthStencilState(state, m_currentDepthStencilState.stencilId);
		}
	}
}

void DRenderStateMgr11::RefreshBlendStencilState()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	if (m_currentBlendState.enableBlend == false)
	{
		if (m_disableBlendState == NULL)
		{
			ID3D11BlendState* state = 0;
			HRESULT result = CreateDisableBlendState(&state);
			if (!FAILED(result))
			{
				m_disableBlendState = state;
				m_deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
			}
		}
		else
		{
			m_deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
		}
	}
	else
	{

		unsigned long key = m_currentBlendState.GetKey();
		
		if (m_blendStates.find(key) != m_blendStates.end())
		{
			ID3D11BlendState* state = m_blendStates.at(key);
			m_deviceContext->OMSetBlendState(state, blendFactor, 0xffffffff);
		}
		else
		{
			ID3D11BlendState* state;
			HRESULT result = CreateBlendState(m_currentBlendState, &state);
			if (!FAILED(result))
			{
				m_blendStates.insert(std::pair<unsigned long, ID3D11BlendState*>(key, state));
				m_deviceContext->OMSetBlendState(state, blendFactor, 0xffffffff);
			}
		}
	}
}

void DRenderStateMgr11::RefreshRasterizerState()
{
	unsigned int key = m_currentRasterizerState.GetKey();
	if (m_rasterizerStates.find(key) != m_rasterizerStates.end())
	{
		ID3D11RasterizerState* state = m_rasterizerStates.at(key);
		m_deviceContext->RSSetState(state);
	}
	else
	{
		ID3D11RasterizerState* state;
		HRESULT result = CreateRasterizerState(m_currentRasterizerState, &state);
		if (!FAILED(result))
		{
			m_rasterizerStates.insert(std::pair<unsigned int, ID3D11RasterizerState*>(key, state));
			m_deviceContext->RSSetState(state);
		}
	}
}

D3D11_COMPARISON_FUNC DRenderStateMgr11::GetComparisonFunc(DRSCompareFunc func)
{
	switch (func)
	{
	case DRSCompareFunc_Always:
		return D3D11_COMPARISON_ALWAYS;
	case DRSCompareFunc_Equal:
		return D3D11_COMPARISON_EQUAL;
	case DRSCompareFunc_GEqual:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case DRSCompareFunc_Greater:
		return D3D11_COMPARISON_GREATER;
	case DRSCompareFunc_LEqual:
		return D3D11_COMPARISON_LESS_EQUAL;
	case DRSCompareFunc_Less:
		return D3D11_COMPARISON_LESS;
	case DRSCompareFunc_Never:
		return D3D11_COMPARISON_NEVER;
	case DRSCompareFunc_NotEqual:
		return D3D11_COMPARISON_NOT_EQUAL;
	default:
		return D3D11_COMPARISON_LESS_EQUAL;
	}
}

D3D11_BLEND_OP DRenderStateMgr11::GetBlendOp(DRSBlendOp op)
{
	switch (op)
	{
	case DRSBlendOp_Add:
		return D3D11_BLEND_OP_ADD;
	case DRSBlendOp_Sub:
		return D3D11_BLEND_OP_SUBTRACT;
	case DRSBlendOp_RevSub:
		return D3D11_BLEND_OP_REV_SUBTRACT;
	case DRSBlendOp_Min:
		return D3D11_BLEND_OP_MIN;
	case DRSBlendOp_Max:
		return D3D11_BLEND_OP_MAX;
	default:
		return D3D11_BLEND_OP_ADD;
	}
}

D3D11_BLEND DRenderStateMgr11::GetBlendFactor(DRSBlendFactor factor)
{
	switch (factor)
	{
	case DRSBlendFactor_One:
		return D3D11_BLEND_ONE;
	case DRSBlendFactor_Zero:
		return D3D11_BLEND_ZERO;
	case DRSBlendFactor_SrcColor:
		return D3D11_BLEND_SRC_COLOR;
	case DRSBlendFactor_SrcAlpha:
		return D3D11_BLEND_SRC_ALPHA;
	case DRSBlendFactor_DstColor:
		return D3D11_BLEND_DEST_COLOR;
	case DRSBlendFactor_DstAlpha:
		return D3D11_BLEND_DEST_ALPHA;
	case DRSBlendFactor_OneMinusSrcColor:
		return D3D11_BLEND_INV_SRC_COLOR;
	case DRSBlendFactor_OneMinusSrcAlpha:
		return D3D11_BLEND_INV_SRC_ALPHA;
	case DRSBlendFactor_OneMinusDstColor:
		return D3D11_BLEND_INV_DEST_COLOR;
	case DRSBlendFactor_OneMinusDstAlpha:
		return D3D11_BLEND_INV_DEST_ALPHA;
	default:
		return D3D11_BLEND_ZERO;
	}
}

D3D11_STENCIL_OP DRenderStateMgr11::GetStencilOp(DRSStencilOp op)
{
	switch (op)
	{
	case DRSStencilOp_Keep:
		return D3D11_STENCIL_OP_KEEP;
	case DRSStencilOp_Zero:
		return D3D11_STENCIL_OP_ZERO;
	case DRSStencilOp_Replace:
		return D3D11_STENCIL_OP_REPLACE;
	case DRSStencilOp_IncrementSaturate:
		return D3D11_STENCIL_OP_INCR_SAT;
	case DRSStencilOp_DecrementSaturate:
		return D3D11_STENCIL_OP_DECR_SAT;
	case DRSStencilOp_Invert:
		return D3D11_STENCIL_OP_INVERT;
	case DRSStencilOp_IncrementWrap:
		return D3D11_STENCIL_OP_INCR;
	case DRSStencilOp_DecrementWrap:
		return D3D11_STENCIL_OP_DECR;
	default:
		return D3D11_STENCIL_OP_KEEP;
	}
}

D3D11_FILL_MODE DRenderStateMgr11::GetFillMode(DFillMode fillMode)
{
	if (fillMode == DFillMode_Solid)
		return D3D11_FILL_SOLID;
	else if (fillMode == DFillMode_WireFrame)
		return D3D11_FILL_WIREFRAME;
	return D3D11_FILL_SOLID;
}

D3D11_CULL_MODE DRenderStateMgr11::GetCullMode(DCullMode cullMode)
{
	if (cullMode == DCullMode_Off)
		return D3D11_CULL_NONE;
	else if (cullMode == DCullMode_Back)
		return D3D11_CULL_BACK;
	else if (cullMode == DCullMode_Front)
		return D3D11_CULL_FRONT;
	return D3D11_CULL_BACK;
}

HRESULT DRenderStateMgr11::CreateRasterizerState(RasterizerState statedesc, ID3D11RasterizerState ** state)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AntialiasedLineEnable = false;
	desc.CullMode = GetCullMode(statedesc.cullMode);
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = GetFillMode(statedesc.fillMode);
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	HRESULT result = m_device->CreateRasterizerState(&desc, state);

	return result;
}

HRESULT DRenderStateMgr11::CreateDepthStencilState(DepthStencilState desc, ID3D11DepthStencilState ** state)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	if(desc.zwrite)
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	else
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	depthStencilDesc.DepthFunc = GetComparisonFunc(desc.ztest);

	depthStencilDesc.StencilEnable = desc.enableStencil;
	if (!desc.enableStencil) 
	{
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}
	else
	{
		depthStencilDesc.StencilReadMask = desc.stencilReadMask;
		depthStencilDesc.StencilWriteMask = desc.stencilWriteMask;

		depthStencilDesc.FrontFace.StencilFailOp = GetStencilOp(desc.stencilFailOp);
		depthStencilDesc.FrontFace.StencilDepthFailOp = GetStencilOp(desc.stencilZFailOp);
		depthStencilDesc.FrontFace.StencilPassOp = GetStencilOp(desc.stencilPassOp);
		depthStencilDesc.FrontFace.StencilFunc = GetComparisonFunc(desc.stencilComp);

		depthStencilDesc.BackFace.StencilFailOp = GetStencilOp(desc.stencilFailOp);
		depthStencilDesc.BackFace.StencilDepthFailOp = GetStencilOp(desc.stencilZFailOp);
		depthStencilDesc.BackFace.StencilPassOp = GetStencilOp(desc.stencilPassOp);
		depthStencilDesc.BackFace.StencilFunc = GetComparisonFunc(desc.stencilComp);
	}

	HRESULT result = m_device->CreateDepthStencilState(&depthStencilDesc, state);
	return result;
}

HRESULT DRenderStateMgr11::CreateBlendState(BlendState desc, ID3D11BlendState ** state)
{
	D3D11_BLEND_DESC blenddesc;
	ZeroMemory(&blenddesc, sizeof(blenddesc));

	blenddesc.RenderTarget[0].BlendEnable = TRUE;
	blenddesc.RenderTarget[0].BlendOp = GetBlendOp(desc.blendOp);
	blenddesc.RenderTarget[0].BlendOpAlpha = GetBlendOp(desc.blendOp);
	blenddesc.RenderTarget[0].DestBlend = GetBlendFactor(desc.dstfactor);
	blenddesc.RenderTarget[0].DestBlendAlpha = GetBlendFactor(desc.dstfactor);
	blenddesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	blenddesc.RenderTarget[0].SrcBlend = GetBlendFactor(desc.srcfactor);
	blenddesc.RenderTarget[0].SrcBlendAlpha = GetBlendFactor(desc.srcfactor);

	HRESULT result = m_device->CreateBlendState(&blenddesc, state);
	return result;
}

HRESULT DRenderStateMgr11::CreateDisableBlendState(ID3D11BlendState ** state)
{
	D3D11_BLEND_DESC blenddesc;
	ZeroMemory(&blenddesc, sizeof(blenddesc));

	blenddesc.RenderTarget[0].BlendEnable = FALSE;
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = m_device->CreateBlendState(&blenddesc, state);
	return result;
}
#endif