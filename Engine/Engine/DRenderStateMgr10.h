#pragma once
#include "D3D10Core.h"
#include <map>

/*
	Direct3D10渲染状态管理器
*/
class DRenderStateMgr10 : public DRenderStateMgr
{
private:
	struct DepthStencilState10
	{
	public:
		unsigned long GetKey();

	public:
		bool zwrite;
		UINT stencilId;
		DRSCompareFunc ztest;
	};

private:
	struct BlendState10
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
	DRenderStateMgr10(ID3D10Device*);
	~DRenderStateMgr10();

	virtual void Init();
	virtual void Release();
	/*设置Cull模式*/
	virtual void SetCullMode(DCullMode cullmode);
	/*开启/关闭深度写入*/
	virtual void SetZWriteEnable(bool zwrite);
	/*设置深度测试模式*/
	virtual void SetZTestFunc(DRSCompareFunc ztest);
	virtual void SetBlendOp(DRSBlendOp);
	virtual void SetBlendEnable(bool);
	virtual void SetBlendSrcFactor(DRSBlendFactor);
	virtual void SetBlendDstFactor(DRSBlendFactor);

private:
	void ChangeCullMode(DCullMode);
	void RefreshDepthStencilState();
	void RefreshBlendStencilState();
	void InitRasterizerStates();
	D3D10_COMPARISON_FUNC GetComparisonFunc(DRSCompareFunc);
	D3D10_BLEND_OP GetBlendOp(DRSBlendOp);
	D3D10_BLEND GetBlendFactor(DRSBlendFactor);
	HRESULT CreateRasterizerState(D3D10_CULL_MODE, ID3D10RasterizerState**);
	HRESULT CreateDepthStencilState(DepthStencilState10, ID3D10DepthStencilState**);
	HRESULT CreateBlendState(BlendState10, ID3D10BlendState**);
	HRESULT CreateDisableBlendState(ID3D10BlendState**);

private:
	ID3D10Device* m_device;
	std::map<DCullMode, ID3D10RasterizerState*> m_rasterizerStates;
	std::map<unsigned long, ID3D10DepthStencilState*> m_depthStencilStates;
	std::map<unsigned long, ID3D10BlendState*> m_blendStates;
	ID3D10BlendState* m_disableBlendState;

	DCullMode m_currentMode;
	DepthStencilState10 m_currentDepthStencilState;
	BlendState10 m_currentBlendState;
};

