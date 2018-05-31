#pragma once
#include "DResObject.h"
#include "DMath.h"
#include "DColor.h"
#include "DGLCore.h"
#include <fstream>
#include <D3DX11.h>
#include <d3dcompiler.h>

class DShader : public DResObject
{
private:
	DShader();

public:
	~DShader();

	virtual void Destroy();

	static DShader* Create(WCHAR*, WCHAR*);

private:
	DShaderBuffer* m_ShaderBuffer;
};