#pragma once
#include "D3D10Core.h"
#include "D3DCore.h"
#include <map>

/*
	Direct3D10渲染状态管理器
*/
class DRenderStateMgr10 : public IRenderStateMgr
{
public:
	DRenderStateMgr10(ID3D10Device*);
	~DRenderStateMgr10();

	virtual void Init();
	virtual void Release();
	/*设置Cull模式*/
	virtual void SetCullMode(DCullMode cullmode);
	/*设置填充模式*/
	virtual void SetFillMode(DFillMode);
	/*开启/关闭深度写入*/
	virtual void SetZWriteEnable(bool zwrite);
	/*设置深度测试模式*/
	virtual void SetZTestFunc(DRSCompareFunc ztest);
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
	D3D10_COMPARISON_FUNC GetComparisonFunc(DRSCompareFunc);
	D3D10_BLEND_OP GetBlendOp(DRSBlendOp);
	D3D10_BLEND GetBlendFactor(DRSBlendFactor);
	D3D10_STENCIL_OP GetStencilOp(DRSStencilOp);
	D3D10_FILL_MODE GetFillMode(DFillMode);
	D3D10_CULL_MODE GetCullMode(DCullMode);
	HRESULT CreateRasterizerState(RasterizerState, ID3D10RasterizerState**);
	HRESULT CreateDepthStencilState(DepthStencilState, ID3D10DepthStencilState**);
	HRESULT CreateBlendState(BlendState, ID3D10BlendState**);
	HRESULT CreateDisableBlendState(ID3D10BlendState**);

private:
	ID3D10Device* m_device;
	std::map<unsigned int, ID3D10RasterizerState*> m_rasterizerStates;
	std::map<unsigned long, ID3D10DepthStencilState*> m_depthStencilStates;
	std::map<unsigned long, ID3D10BlendState*> m_blendStates;
	ID3D10BlendState* m_disableBlendState;

	RasterizerState m_currentRasterizerState;
	DepthStencilState m_currentDepthStencilState;
	BlendState m_currentBlendState;
};

