#pragma once
#include "DResObject.h"
#include "DMath.h"
#include "DColor.h"
#include "DGLCore.h"
#include "DTexture.h"

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
	void ApplyRes(const LPCSTR key, DTexture*) const;
	void Draw();

	static DShader* Create(WCHAR*, WCHAR*);

private:
	DShaderRes* m_shaderRes;
};