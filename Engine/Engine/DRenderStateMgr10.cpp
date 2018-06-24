#include "DRenderStateMgr10.h"

DRenderStateMgr10::DRenderStateMgr10(ID3D10Device * device)
{
	m_device = device;
}

DRenderStateMgr10::~DRenderStateMgr10()
{
}

void DRenderStateMgr10::Init()
{
	if (m_device == NULL)
		return;
	InitRasterizerStates();

	ChangeCullMode(DCullMode_Back);

	m_currentDepthStencilState.ztest = DRSCompareFunc_LEqual;
	m_currentDepthStencilState.zwrite = true;
	m_currentDepthStencilState.stencilId = 1;

	RefreshDepthStencilState();
}

void DRenderStateMgr10::Release()
{
	m_device = NULL;
	std::map<DCullMode, ID3D10RasterizerState*>::iterator  iter;
	for (iter = m_rasterizerStates.begin(); iter != m_rasterizerStates.end(); iter++)
	{
		if (iter->second != NULL)
		{
			iter->second->Release();
		}
	}
	m_rasterizerStates.clear();

	std::map<unsigned long, ID3D10DepthStencilState*>::iterator  diter;
	for (diter = m_depthStencilStates.begin(); diter != m_depthStencilStates.end(); diter++)
	{
		if (diter->second != NULL)
		{
			diter->second->Release();
		}
	}
	m_depthStencilStates.clear();
}

void DRenderStateMgr10::SetCullMode(DCullMode cullMode)
{
	if (m_device == NULL)
		return;
	if (m_currentMode == cullMode)
		return;
	ChangeCullMode(cullMode);
}

void DRenderStateMgr10::SetZWriteEnable(bool zwrite)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.zwrite == zwrite)
		return;
	m_currentDepthStencilState.zwrite = zwrite;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetZTestFunc(DRSCompareFunc ztest)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.ztest == ztest)
		return;
	m_currentDepthStencilState.ztest = ztest;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::ChangeCullMode(DCullMode cullMode)
{
	if (m_rasterizerStates.find(cullMode) != m_rasterizerStates.end())
	{
		ID3D10RasterizerState* state = m_rasterizerStates.at(cullMode);
		m_device->RSSetState(state);

		m_currentMode = cullMode;
	}
}

void DRenderStateMgr10::RefreshDepthStencilState()
{
	unsigned long key = m_currentDepthStencilState.GetKey();
	if (m_depthStencilStates.find(key) != m_depthStencilStates.end())
	{
		ID3D10DepthStencilState* state = m_depthStencilStates.at(key);
		m_device->OMSetDepthStencilState(state, m_currentDepthStencilState.stencilId);
	}
	else
	{
		ID3D10DepthStencilState* state;
		HRESULT result = CreateDepthStencilState(m_currentDepthStencilState, &state);
		if (!FAILED(result))
		{
			m_depthStencilStates.insert(std::pair<unsigned long, ID3D10DepthStencilState*>(key, state));
			m_device->OMSetDepthStencilState(state, m_currentDepthStencilState.stencilId);
		}
	}
}

void DRenderStateMgr10::InitRasterizerStates()
{
	HRESULT result;
	ID3D10RasterizerState* state = 0;
	result = CreateRasterizerState(D3D10_CULL_BACK, &state);
	if (!FAILED(result))
	{
		m_rasterizerStates.insert(std::pair<DCullMode, ID3D10RasterizerState*>(DCullMode_Back, state));
	}

	state = 0;
	result = CreateRasterizerState(D3D10_CULL_FRONT, &state);
	if (!FAILED(result))
	{
		m_rasterizerStates.insert(std::pair<DCullMode, ID3D10RasterizerState*>(DCullMode_Front, state));
	}

	state = 0;
	result = CreateRasterizerState(D3D10_CULL_NONE, &state);
	if (!FAILED(result))
	{
		m_rasterizerStates.insert(std::pair<DCullMode, ID3D10RasterizerState*>(DCullMode_Off, state));
	}
}

HRESULT DRenderStateMgr10::CreateRasterizerState(D3D10_CULL_MODE cullmode, ID3D10RasterizerState ** out)
{
	D3D10_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AntialiasedLineEnable = false;
	desc.CullMode = cullmode;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D10_FILL_SOLID;
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	HRESULT result = m_device->CreateRasterizerState(&desc, out);
	return result;
}

HRESULT DRenderStateMgr10::CreateDepthStencilState(DepthStencilState10 desc, ID3D10DepthStencilState ** state)
{
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	if (desc.zwrite)
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	else
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;

	switch (desc.ztest)
	{
	case DRSCompareFunc_Always:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_ALWAYS;
		break;
	case DRSCompareFunc_Equal:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_EQUAL;
		break;
	case DRSCompareFunc_GEqual:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_GREATER_EQUAL;
		break;
	case DRSCompareFunc_Greater:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_GREATER;
		break;
	case DRSCompareFunc_LEqual:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS_EQUAL;
		break;
	case DRSCompareFunc_Less:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
		break;
	case DRSCompareFunc_Never:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_NEVER;
		break;
	case DRSCompareFunc_NotEqual:
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_NOT_EQUAL;
		break;
	default:
		break;
	}

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

	HRESULT result = m_device->CreateDepthStencilState(&depthStencilDesc, state);
	return result;
}

unsigned long DRenderStateMgr10::DepthStencilState10::GetKey()
{
	unsigned long key = 0;
	if (zwrite)
		key = 1L << 4;
	unsigned long func = (unsigned long)ztest;
	key = key | func;

	return key;
}
