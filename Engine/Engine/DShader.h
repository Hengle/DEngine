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
	void GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int& poffset, int& plength, int& stype) const;
	bool HasProperty(const LPCSTR key) const;
	DShaderBuffer* GetShaderBuffer();
	//DShaderParam* GetParam();

	static DShader* Create(WCHAR*, WCHAR*);

private:
	DShaderBuffer* m_shaderBuffer;
};