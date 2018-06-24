#pragma once
#include "D3D10Core.h"
#include <map>

class DRenderStateMgr10 : public DRenderStateMgr
{
public:
	DRenderStateMgr10(ID3D10Device*);
	~DRenderStateMgr10();

	virtual void Init();
	virtual void Release();
	virtual void SetCullMode(DCullMode);
	virtual void SetZWriteEnable(bool);
	virtual void SetZTestFunc(DRSCompareFunc);

private:
	void ChangeCullMode(DCullMode);
	void InitRasterizerStates();
	HRESULT CreateRasterizerState(D3D10_CULL_MODE, ID3D10RasterizerState**);

private:
	ID3D10Device* m_device;
	std::map<DCullMode, ID3D10RasterizerState*> m_rasterizerStates;

	DCullMode m_currentMode;
};

