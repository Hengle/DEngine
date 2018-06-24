#pragma once
#include "D3D11Core.h"
#include <map>

class DRenderStateMgr11 : public DRenderStateMgr
{
private:
	struct DepthStencilState11
	{
	public:
		unsigned long GetKey();

	public:
		bool zwrite;
		UINT stencilId;
		DRSCompareFunc ztest;
	};

public:
	DRenderStateMgr11(ID3D11Device*, ID3D11DeviceContext*);
	~DRenderStateMgr11();

	virtual void Init();
	virtual void Release();
	virtual void SetCullMode(DCullMode);
	virtual void SetZWriteEnable(bool);
	virtual void SetZTestFunc(DRSCompareFunc);

private:
	void ChangeCullMode(DCullMode);
	void RefreshDepthStencilState();
	void InitRasterizerStates();
	HRESULT CreateRasterizerState(D3D11_CULL_MODE, ID3D11RasterizerState**);
	HRESULT CreateDepthStencilState(DepthStencilState11, ID3D11DepthStencilState**);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	std::map<DCullMode, ID3D11RasterizerState*> m_rasterizerStates;
	std::map<unsigned long, ID3D11DepthStencilState*> m_depthStencilStates;

	DCullMode m_currentMode;
	DepthStencilState11 m_currentDepthStencilState;
};

