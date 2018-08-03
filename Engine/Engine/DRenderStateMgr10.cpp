#ifdef _DGAPI_D3D10
#include "DRenderStateMgr10.h"

DRenderStateMgr10::DRenderStateMgr10(ID3D10Device * device)
{
	m_device = device;
	m_disableBlendState = 0;
}

DRenderStateMgr10::~DRenderStateMgr10()
{
}

void DRenderStateMgr10::Init()
{
	if (m_device == NULL)
		return;
	//InitRasterizerStates();

	//ChangeCullMode(DCullMode_Back);
	m_currentRasterizerState.fillMode = DFillMode_Solid;
	m_currentRasterizerState.cullMode = DCullMode_Back;

	RefreshRasterizerState();

	m_currentDepthStencilState.ztest = DRSCompareFunc_LEqual;
	m_currentDepthStencilState.zwrite = true;
	m_currentDepthStencilState.stencilId = 0;
	m_currentDepthStencilState.enableStencil = false;

	RefreshDepthStencilState();

	m_currentBlendState.enableBlend = false;
	RefreshBlendState();
}

void DRenderStateMgr10::Release()
{
	m_device = NULL;
	std::map<unsigned int, ID3D10RasterizerState*>::iterator  riter;
	for (riter = m_rasterizerStates.begin(); riter != m_rasterizerStates.end(); riter++)
	{
		if (riter->second != NULL)
		{
			riter->second->Release();
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

	std::map<unsigned long, ID3D10BlendState*>::iterator  biter;
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

void DRenderStateMgr10::SetCullMode(DCullMode cullMode)
{
	if (m_device == NULL)
		return;
	if (m_currentRasterizerState.cullMode == cullMode)
		return;
	m_currentRasterizerState.cullMode = cullMode;
	RefreshRasterizerState();
}

void DRenderStateMgr10::SetFillMode(DFillMode fillMode)
{
	if (m_device == NULL)
		return;
	if (m_currentRasterizerState.fillMode == fillMode)
		return;
	m_currentRasterizerState.fillMode = fillMode;
	RefreshRasterizerState();
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

void DRenderStateMgr10::SetBlendOp(DRSBlendOp op)
{
	if (m_device == NULL)
		return;
	if (m_currentBlendState.blendOp == op)
		return;
	m_currentBlendState.blendOp = op;
	if (m_currentBlendState.enableBlend == false)
		return;
	RefreshBlendState();
}

void DRenderStateMgr10::SetBlendEnable(bool enableBlend)
{
	if (m_device == NULL)
		return;
	if (m_currentBlendState.enableBlend == enableBlend)
		return;
	m_currentBlendState.enableBlend = enableBlend;
	RefreshBlendState();
}

void DRenderStateMgr10::SetBlendSrcFactor(DRSBlendFactor factor)
{
	if (m_device == NULL)
		return;
	if (m_currentBlendState.srcfactor == factor)
		return;
	m_currentBlendState.srcfactor = factor;
	if (m_currentBlendState.enableBlend == false)
		return;
	RefreshBlendState();
}

void DRenderStateMgr10::SetBlendDstFactor(DRSBlendFactor factor)
{
	if (m_device == NULL)
		return;
	if (m_currentBlendState.dstfactor == factor)
		return;
	m_currentBlendState.dstfactor = factor;
	if (m_currentBlendState.enableBlend == false)
		return;
	RefreshBlendState();
}

void DRenderStateMgr10::SetBlendFactor(DRSBlendFactor src, DRSBlendFactor dst)
{
	if (m_device == NULL)
		return;
	if (m_currentBlendState.srcfactor == src && m_currentBlendState.dstfactor == dst)
		return;
	m_currentBlendState.srcfactor = src;
	m_currentBlendState.dstfactor = dst;
	if (m_currentBlendState.enableBlend == false)
		return;
	RefreshBlendState();
}

void DRenderStateMgr10::SetStencilRefId(UINT stencilId)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.stencilId == stencilId)
		return;
	m_currentDepthStencilState.stencilId = stencilId;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilEnable(bool enableStencil)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.enableStencil == enableStencil)
		return;
	m_currentDepthStencilState.enableStencil = enableStencil;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilReadMask(unsigned short readMask)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.stencilReadMask == readMask)
		return;
	m_currentDepthStencilState.stencilReadMask = readMask;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilWriteMask(unsigned short writeMask)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.stencilWriteMask == writeMask)
		return;
	m_currentDepthStencilState.stencilWriteMask = writeMask;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilComparisonFunc(DRSCompareFunc compFunc)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.stencilComp == compFunc)
		return;
	m_currentDepthStencilState.stencilComp = compFunc;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilPassOp(DRSStencilOp stencilPass)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.stencilPassOp == stencilPass)
		return;
	m_currentDepthStencilState.stencilPassOp = stencilPass;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilFailOp(DRSStencilOp stencilFail)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.stencilFailOp == stencilFail)
		return;
	m_currentDepthStencilState.stencilFailOp = stencilFail;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilZFailOp(DRSStencilOp stencilZFail)
{
	if (m_device == NULL)
		return;
	if (m_currentDepthStencilState.stencilZFailOp == stencilZFail)
		return;
	m_currentDepthStencilState.stencilZFailOp = stencilZFail;
	if (m_currentDepthStencilState.enableStencil == false)
		return;
	RefreshDepthStencilState();
}

void DRenderStateMgr10::SetStencilOp(DRSStencilOp stencilFail, DRSStencilOp zFail, DRSStencilOp stencilPass)
{
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

void DRenderStateMgr10::RefreshBlendState()
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
			ID3D10BlendState* state = 0;
			HRESULT result = CreateDisableBlendState(&state);
			if (!FAILED(result))
			{
				m_disableBlendState = state;
				m_device->OMSetBlendState(state, blendFactor, 0xffffffff);
			}
		}
		else
		{
			m_device->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
		}
	}
	else
	{

		unsigned long key = m_currentBlendState.GetKey();

		if (m_blendStates.find(key) != m_blendStates.end())
		{
			ID3D10BlendState* state = m_blendStates.at(key);
			m_device->OMSetBlendState(state, blendFactor, 0xffffffff);
		}
		else
		{
			ID3D10BlendState* state;
			HRESULT result = CreateBlendState(m_currentBlendState, &state);
			if (!FAILED(result))
			{
				m_blendStates.insert(std::pair<unsigned long, ID3D10BlendState*>(key, state));
				m_device->OMSetBlendState(state, blendFactor, 0xffffffff);
			}
		}
	}
}

void DRenderStateMgr10::RefreshRasterizerState()
{
	unsigned int key = m_currentRasterizerState.GetKey();
	if (m_rasterizerStates.find(key) != m_rasterizerStates.end())
	{
		ID3D10RasterizerState* state = m_rasterizerStates.at(key);
		m_device->RSSetState(state);
	}
	else
	{
		ID3D10RasterizerState* state;
		HRESULT result = CreateRasterizerState(m_currentRasterizerState, &state);
		if (!FAILED(result))
		{
			m_rasterizerStates.insert(std::pair<unsigned int, ID3D10RasterizerState*>(key, state));
			m_device->RSSetState(state);
		}
	}
}

D3D10_COMPARISON_FUNC DRenderStateMgr10::GetComparisonFunc(DRSCompareFunc func)
{
	switch (func)
	{
	case DRSCompareFunc_Always:
		return D3D10_COMPARISON_ALWAYS;
	case DRSCompareFunc_Equal:
		return D3D10_COMPARISON_EQUAL;
	case DRSCompareFunc_GEqual:
		return D3D10_COMPARISON_GREATER_EQUAL;
	case DRSCompareFunc_Greater:
		return D3D10_COMPARISON_GREATER;
	case DRSCompareFunc_LEqual:
		return D3D10_COMPARISON_LESS_EQUAL;
	case DRSCompareFunc_Less:
		return D3D10_COMPARISON_LESS;
	case DRSCompareFunc_Never:
		return D3D10_COMPARISON_NEVER;
	case DRSCompareFunc_NotEqual:
		return D3D10_COMPARISON_NOT_EQUAL;
	default:
		return D3D10_COMPARISON_LESS_EQUAL;
	}
}

D3D10_BLEND_OP DRenderStateMgr10::GetBlendOp(DRSBlendOp op)
{
	switch (op)
	{
	case DRSBlendOp_Add:
		return D3D10_BLEND_OP_ADD;
	case DRSBlendOp_Sub:
		return D3D10_BLEND_OP_SUBTRACT;
	case DRSBlendOp_RevSub:
		return D3D10_BLEND_OP_REV_SUBTRACT;
	case DRSBlendOp_Min:
		return D3D10_BLEND_OP_MIN;
	case DRSBlendOp_Max:
		return D3D10_BLEND_OP_MAX;
	default:
		return D3D10_BLEND_OP_ADD;
	}
}

D3D10_BLEND DRenderStateMgr10::GetBlendFactor(DRSBlendFactor factor)
{
	switch (factor)
	{
	case DRSBlendFactor_One:
		return D3D10_BLEND_ONE;
	case DRSBlendFactor_Zero:
		return D3D10_BLEND_ZERO;
	case DRSBlendFactor_SrcColor:
		return D3D10_BLEND_SRC_COLOR;
	case DRSBlendFactor_SrcAlpha:
		return D3D10_BLEND_SRC_ALPHA;
	case DRSBlendFactor_DstColor:
		return D3D10_BLEND_DEST_COLOR;
	case DRSBlendFactor_DstAlpha:
		return D3D10_BLEND_DEST_ALPHA;
	case DRSBlendFactor_OneMinusSrcColor:
		return D3D10_BLEND_INV_SRC_COLOR;
	case DRSBlendFactor_OneMinusSrcAlpha:
		return D3D10_BLEND_INV_SRC_ALPHA;
	case DRSBlendFactor_OneMinusDstColor:
		return D3D10_BLEND_INV_DEST_COLOR;
	case DRSBlendFactor_OneMinusDstAlpha:
		return D3D10_BLEND_INV_DEST_ALPHA;
	default:
		return D3D10_BLEND_ZERO;
	}
}

D3D10_STENCIL_OP DRenderStateMgr10::GetStencilOp(DRSStencilOp op)
{
	switch (op)
	{
	case DRSStencilOp_Keep:
		return D3D10_STENCIL_OP_KEEP;
	case DRSStencilOp_Zero:
		return D3D10_STENCIL_OP_ZERO;
	case DRSStencilOp_Replace:
		return D3D10_STENCIL_OP_REPLACE;
	case DRSStencilOp_IncrementSaturate:
		return D3D10_STENCIL_OP_INCR_SAT;
	case DRSStencilOp_DecrementSaturate:
		return D3D10_STENCIL_OP_DECR_SAT;
	case DRSStencilOp_Invert:
		return D3D10_STENCIL_OP_INVERT;
	case DRSStencilOp_IncrementWrap:
		return D3D10_STENCIL_OP_INCR;
	case DRSStencilOp_DecrementWrap:
		return D3D10_STENCIL_OP_DECR;
	default:
		return D3D10_STENCIL_OP_KEEP;
	}
}

D3D10_FILL_MODE DRenderStateMgr10::GetFillMode(DFillMode fillMode)
{
	if (fillMode == DFillMode_Solid)
		return D3D10_FILL_SOLID;
	else if (fillMode == DFillMode_WireFrame)
		return D3D10_FILL_WIREFRAME;
	return D3D10_FILL_SOLID;
}

D3D10_CULL_MODE DRenderStateMgr10::GetCullMode(DCullMode cullMode)
{
	if (cullMode == DCullMode_Off)
		return D3D10_CULL_NONE;
	else if (cullMode == DCullMode_Back)
		return D3D10_CULL_BACK;
	else if (cullMode == DCullMode_Front)
		return D3D10_CULL_FRONT;
	return D3D10_CULL_BACK;
}

HRESULT DRenderStateMgr10::CreateRasterizerState(RasterizerState statedesc, ID3D10RasterizerState ** state)
{
	D3D10_RASTERIZER_DESC desc;
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

HRESULT DRenderStateMgr10::CreateDepthStencilState(DepthStencilState desc, ID3D10DepthStencilState ** state)
{
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	if (desc.zwrite)
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	else
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;

	depthStencilDesc.DepthFunc = GetComparisonFunc(desc.ztest);

	depthStencilDesc.StencilEnable = desc.enableStencil;
	if (!desc.enableStencil)
	{
		depthStencilDesc.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

		depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
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

HRESULT DRenderStateMgr10::CreateBlendState(BlendState desc, ID3D10BlendState ** state)
{
	D3D10_BLEND_DESC blenddesc;
	ZeroMemory(&blenddesc, sizeof(blenddesc));

	blenddesc.BlendEnable[0] = true;
	blenddesc.BlendOp = GetBlendOp(desc.blendOp);
	blenddesc.BlendOpAlpha = GetBlendOp(desc.blendOp);
	blenddesc.DestBlend = GetBlendFactor(desc.dstfactor);
	blenddesc.DestBlendAlpha = GetBlendFactor(desc.dstfactor);
	blenddesc.RenderTargetWriteMask[0] = 0x0f;
	blenddesc.SrcBlend = GetBlendFactor(desc.srcfactor);
	blenddesc.SrcBlendAlpha = GetBlendFactor(desc.srcfactor);

	HRESULT result = m_device->CreateBlendState(&blenddesc, state);
	return result;
}

HRESULT DRenderStateMgr10::CreateDisableBlendState(ID3D10BlendState ** state)
{
	D3D10_BLEND_DESC blenddesc;
	ZeroMemory(&blenddesc, sizeof(blenddesc));

	blenddesc.BlendEnable[0] = false;
	blenddesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = m_device->CreateBlendState(&blenddesc, state);
	return result;
}
#endif