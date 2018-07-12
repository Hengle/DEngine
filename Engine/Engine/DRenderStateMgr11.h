#pragma once
#include "D3D11Core.h"
#include "D3DCore.h"
#include <map>

/*
	Direct3D11渲染状态管理器
*/
class DRenderStateMgr11 : public IRenderStateMgr
{
public:
	DRenderStateMgr11(ID3D11Device*, ID3D11DeviceContext*);
	~DRenderStateMgr11();

	virtual void Init();
	virtual void Release();
	/*设置Cull模式*/
	virtual void SetCullMode(DCullMode);
	/*设置填充模式*/
	virtual void SetFillMode(DFillMode);
	/*开启/关闭深度写入*/
	virtual void SetZWriteEnable(bool);
	/*设置深度测试模式*/
	virtual void SetZTestFunc(DRSCompareFunc);
	/*设置混合Operation*/
	virtual void SetBlendOp(DRSBlendOp);
	/*设置开启/关闭混合*/
	virtual void SetBlendEnable(bool);
	/*设置混合源因子*/
	virtual void SetBlendSrcFactor(DRSBlendFactor);
	/*设置混合目标因子*/
	virtual void SetBlendDstFactor(DRSBlendFactor);
	virtual void SetStencilRefId(UINT);
	virtual void SetStencilEnable(bool);
	virtual void SetStencilReadMask(unsigned short);
	virtual void SetStencilWriteMask(unsigned short);
	virtual void SetStencilComparisonFunc(DRSCompareFunc);
	virtual void SetStencilPassOp(DRSStencilOp);
	virtual void SetStencilFailOp(DRSStencilOp);
	virtual void SetStencilZFailOp(DRSStencilOp);

private:
	void RefreshDepthStencilState();
	void RefreshBlendStencilState();
	void RefreshRasterizerState();
	D3D11_COMPARISON_FUNC GetComparisonFunc(DRSCompareFunc);
	D3D11_BLEND_OP GetBlendOp(DRSBlendOp);
	D3D11_BLEND GetBlendFactor(DRSBlendFactor);
	D3D11_STENCIL_OP GetStencilOp(DRSStencilOp);
	D3D11_FILL_MODE GetFillMode(DFillMode);
	D3D11_CULL_MODE GetCullMode(DCullMode);
	HRESULT CreateRasterizerState(RasterizerState, ID3D11RasterizerState**);
	HRESULT CreateDepthStencilState(DepthStencilState, ID3D11DepthStencilState**);
	HRESULT CreateBlendState(BlendState, ID3D11BlendState**);
	HRESULT CreateDisableBlendState(ID3D11BlendState**);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	std::map<unsigned long, ID3D11DepthStencilState*> m_depthStencilStates;
	std::map<unsigned long, ID3D11BlendState*> m_blendStates;
	std::map<unsigned int, ID3D11RasterizerState*> m_rasterizerStates;
	ID3D11BlendState* m_disableBlendState;

	RasterizerState m_currentRasterizerState;
	DepthStencilState m_currentDepthStencilState;
	BlendState m_currentBlendState;
};

