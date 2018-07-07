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
	InitRasterizerStates();

	ChangeCullMode(DCullMode_Back);

	m_currentDepthStencilState.ztest = DRSCompareFunc_LEqual;
	m_currentDepthStencilState.zwrite = true;
	m_currentDepthStencilState.stencilId = 1;

	RefreshDepthStencilState();

	
	m_currentBlendState.enableBlend = false;
	RefreshBlendStencilState();
}

void DRenderStateMgr11::Release()
{
	m_device = NULL;
	m_deviceContext = NULL;
	std::map<DCullMode, ID3D11RasterizerState*>::iterator  iter;
	for (iter = m_rasterizerStates.begin(); iter != m_rasterizerStates.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
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
	if (m_currentMode == cullMode)
		return;
	ChangeCullMode(cullMode);
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

void DRenderStateMgr11::ChangeCullMode(DCullMode cullMode)
{
	if (m_rasterizerStates.find(cullMode) != m_rasterizerStates.end())
	{
		ID3D11RasterizerState* state = m_rasterizerStates.at(cullMode);
		m_deviceContext->RSSetState(state);

		m_currentMode = cullMode;
	}
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

void DRenderStateMgr11::InitRasterizerStates()
{
	HRESULT result;
	ID3D11RasterizerState* state = 0;
	result = CreateRasterizerState(D3D11_CULL_BACK, &state);
	if (!FAILED(result))
	{
		m_rasterizerStates.insert(std::pair<DCullMode, ID3D11RasterizerState*>(DCullMode_Back, state));
	}

	state = 0;
	result = CreateRasterizerState(D3D11_CULL_FRONT, &state);
	if (!FAILED(result))
	{
		m_rasterizerStates.insert(std::pair<DCullMode, ID3D11RasterizerState*>(DCullMode_Front, state));
	}

	state = 0;
	result = CreateRasterizerState(D3D11_CULL_NONE, &state);
	if (!FAILED(result))
	{
		m_rasterizerStates.insert(std::pair<DCullMode, ID3D11RasterizerState*>(DCullMode_Off, state));
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

HRESULT DRenderStateMgr11::CreateRasterizerState(D3D11_CULL_MODE cullmode, ID3D11RasterizerState ** out)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AntialiasedLineEnable = false;
	desc.CullMode = cullmode;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	HRESULT result = m_device->CreateRasterizerState(&desc, out);
	return result;
}

HRESULT DRenderStateMgr11::CreateDepthStencilState(DepthStencilState11 desc, ID3D11DepthStencilState ** state)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	if(desc.zwrite)
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	else
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	depthStencilDesc.DepthFunc = GetComparisonFunc(desc.ztest);

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT result = m_device->CreateDepthStencilState(&depthStencilDesc, state);
	return result;
}

HRESULT DRenderStateMgr11::CreateBlendState(BlendState11 desc, ID3D11BlendState ** state)
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
	blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = m_device->CreateBlendState(&blenddesc, state);
	return result;
}

unsigned long DRenderStateMgr11::DepthStencilState11::GetKey()
{
	unsigned long key = 0;
	if (zwrite)
		key = 1L << 15;
	unsigned long func = ztest << 12;
	key = key | func;
	if (enableStencil)
	{
		key = key | (stencilComp << 9);
		key = key | (stencilPassOp << 6);
		key = key | (stencilFailOp << 3);
		key = key | (stencilZFailOp);
	}

	return key;
}

unsigned long DRenderStateMgr11::BlendState11::GetKey()
{
	unsigned long key = 0;
	key = blendOp << 8;
	key = key | (srcfactor << 4);
	key = key | dstfactor;
	return key;
}
