#include "D3DCore.h"

unsigned int RasterizerState::GetKey()
{
	unsigned int key = 0;
	key = fillMode << 2;
	key = key | cullMode;
	return key;
}

unsigned long DepthStencilState::GetKey()
{
	unsigned long key = 0;
	if (zwrite)
		key = 1L << 31;
	unsigned long func = ztest << 28;
	key = key | func;
	if (enableStencil)
	{
		key = key | (stencilWriteMask << 20);
		key = key | (stencilReadMask << 12);
		key = key | (stencilComp << 9);
		key = key | (stencilPassOp << 6);
		key = key | (stencilFailOp << 3);
		key = key | (stencilZFailOp);
	}

	return key;
}

unsigned long BlendState::GetKey()
{
	unsigned long key = 0;
	key = blendOp << 8;
	key = key | (srcfactor << 4);
	key = key | dstfactor;
	return key;
}
