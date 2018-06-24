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
}

void DRenderStateMgr10::SetCullMode(DCullMode cullMode)
{
	if (m_device == NULL)
		return;
	if (m_currentMode == cullMode)
		return;
	ChangeCullMode(cullMode);
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
