#pragma once
#ifdef _DGAPI_OPENGL

#include "DOpenGLCore.h"

class DRenderStateMgrOpGL : public IRenderStateMgr
{
public:
	DRenderStateMgrOpGL();
	~DRenderStateMgrOpGL();

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
	virtual void SetBlendFactor(DRSBlendFactor, DRSBlendFactor);
	virtual void SetStencilRefId(UINT);
	virtual void SetStencilEnable(bool);
	virtual void SetStencilReadMask(unsigned short);
	virtual void SetStencilWriteMask(unsigned short);
	virtual void SetStencilComparisonFunc(DRSCompareFunc);
	virtual void SetStencilPassOp(DRSStencilOp);
	virtual void SetStencilFailOp(DRSStencilOp);
	virtual void SetStencilZFailOp(DRSStencilOp);
	virtual void SetStencilOp(DRSStencilOp stencilFail, DRSStencilOp zFail, DRSStencilOp stencilPass);

private:
	void ChangeCullMode(DCullMode);
	void ChangeFillMode(DFillMode);
	void ChangeZWrite(bool);
	void ChangeZTest(DRSCompareFunc);
	void ChangeBlendEnable(bool);
	void ChangeBlendOp(DRSBlendOp);
	void ChangeBlendFactor(DRSBlendFactor src, DRSBlendFactor dst);
	void ChangeStencilEnable(bool);
	void ChangeStencilId(unsigned short);
	void ChangeStencilCompFunc(DRSCompareFunc);
	void ChangeStencilOp(DRSStencilOp, DRSStencilOp, DRSStencilOp);
	void ChangeStencilReadMask(unsigned short);
	void ChangeStencilWriteMask(unsigned short);
	GLenum GetComparisonFunc(DRSCompareFunc);
	GLenum GetBlendOp(DRSBlendOp);
	GLenum GetBlendFactor(DRSBlendFactor);
	GLenum GetStencilOp(DRSStencilOp);

private:
	DCullMode m_currentCullMode;
	DFillMode m_currentFillMode;
	bool m_zwrite;
	DRSCompareFunc m_ztest;
	bool m_enableBlend;
	DRSBlendOp m_blendOp;
	DRSBlendFactor m_blendSrcFactor;
	DRSBlendFactor m_blendDstFactor;
	bool m_enableStencil;
	unsigned short m_stencilId;
	DRSCompareFunc m_stencilComp;
	DRSStencilOp m_stencilPass;
	DRSStencilOp m_stencilFail;
	DRSStencilOp m_stencilZFail;
	unsigned short m_stencilReadMask;
	unsigned short m_stencilWriteMask;
};

#endif