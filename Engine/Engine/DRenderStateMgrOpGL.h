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
	virtual void SetStencilRefId(UINT);
	virtual void SetStencilEnable(bool);
	virtual void SetStencilReadMask(unsigned short);
	virtual void SetStencilWriteMask(unsigned short);
	virtual void SetStencilComparisonFunc(DRSCompareFunc);
	virtual void SetStencilPassOp(DRSStencilOp);
	virtual void SetStencilFailOp(DRSStencilOp);
	virtual void SetStencilZFailOp(DRSStencilOp);
};

#endif