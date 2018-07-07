#pragma once
#include "D3D11Core.h"
#include <map>

/*
	Direct3D11渲染状态管理器
*/
class DRenderStateMgr11 : public IRenderStateMgr
{
private:
	struct DepthStencilState11
	{
	public:
		unsigned long GetKey();

	public:
		bool zwrite;
		bool enableStencil;
		UINT stencilId;
		DRSCompareFunc ztest;
		DRSCompareFunc stencilComp;
		DRSStencilOp stencilPassOp;
		DRSStencilOp stencilFailOp;
		DRSStencilOp stencilZFailOp;
	};

private:
	struct BlendState11
	{
	public:
		unsigned long GetKey();
		
	public:
		bool enableBlend;
		DRSBlendOp blendOp;
		DRSBlendFactor srcfactor;
		DRSBlendFactor dstfactor;
	};

public:
	DRenderStateMgr11(ID3D11Device*, ID3D11DeviceContext*);
	~DRenderStateMgr11();

	virtual void Init();
	virtual void Release();
	/*设置Cull模式*/
	virtual void SetCullMode(DCullMode);
	/*开启/关闭深度写入*/
	virtual void SetZWriteEnable(bool);
	/*设置深度测试模式*/
	virtual void SetZTestFunc(DRSCompareFunc);
	virtual void SetBlendOp(DRSBlendOp);
	virtual void SetBlendEnable(bool);
	virtual void SetBlendSrcFactor(DRSBlendFactor);
	virtual void SetBlendDstFactor(DRSBlendFactor);

private:
	void ChangeCullMode(DCullMode);
	void RefreshDepthStencilState();
	void RefreshBlendStencilState();
	void InitRasterizerStates();
	D3D11_COMPARISON_FUNC GetComparisonFunc(DRSCompareFunc);
	D3D11_BLEND_OP GetBlendOp(DRSBlendOp);
	D3D11_BLEND GetBlendFactor(DRSBlendFactor);
	HRESULT CreateRasterizerState(D3D11_CULL_MODE, ID3D11RasterizerState**);
	HRESULT CreateDepthStencilState(DepthStencilState11, ID3D11DepthStencilState**);
	HRESULT CreateBlendState(BlendState11, ID3D11BlendState**);
	HRESULT CreateDisableBlendState(ID3D11BlendState**);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	std::map<DCullMode, ID3D11RasterizerState*> m_rasterizerStates;
	std::map<unsigned long, ID3D11DepthStencilState*> m_depthStencilStates;
	std::map<unsigned long, ID3D11BlendState*> m_blendStates;
	ID3D11BlendState* m_disableBlendState;

	DCullMode m_currentMode;
	DepthStencilState11 m_currentDepthStencilState;
	BlendState11 m_currentBlendState;
};

