#pragma once
#include "D3D9Core.h"

class DRenderStateMgr9 : public DRenderStateMgr
{
public:
	DRenderStateMgr9(IDirect3DDevice9*);
	~DRenderStateMgr9();

	virtual void Init();
	virtual void Release();
	virtual void SetCullMode(DCullMode);
	virtual void SetZWriteEnable(bool);
	virtual void SetZTestFunc(DRSCompareFunc);

private:
	void ChangeCullMode(DCullMode);
	void ChangeZWrite(bool);
	void ChangeZTest(DRSCompareFunc);
	D3DCMPFUNC GetComparisonFunc(DRSCompareFunc);

private:
	IDirect3DDevice9* m_device;

	DCullMode m_currentMode;
	bool m_zwrite;
	DRSCompareFunc m_ztest;
};

