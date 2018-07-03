#pragma once
#include "D3D9Core.h"

/*
	Direct3D9渲染状态管理器
*/
class DRenderStateMgr9 : public DRenderStateMgr
{
public:
	DRenderStateMgr9(IDirect3DDevice9*);
	~DRenderStateMgr9();

	virtual void Init();
	virtual void Release();
	/*设置Cull模式*/
	virtual void SetCullMode(DCullMode cullmode);
	/*开启/关闭深度写入*/
	virtual void SetZWriteEnable(bool zwrite);
	/*设置深度测试模式*/
	virtual void SetZTestFunc(DRSCompareFunc ztest);

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

