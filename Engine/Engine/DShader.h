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
	unsigned int GetCBufferCount() const;
	unsigned int GetPropertyCount() const;
	void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const;
	bool HasProperty(const LPCSTR key) const;
	void ApplyParams(int cindex, int coffset, int csize, int stype, float* params) const;
	void Draw(int indexCount);
	//DShaderBuffer* GetShaderBuffer();
	//DShaderParam* GetParam();

	static DShader* Create(WCHAR*, WCHAR*);

private:
	DShaderRes* m_shaderRes;
};