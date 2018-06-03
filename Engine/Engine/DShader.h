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
	unsigned int GetPropertyCount(LPCSTR cbuffername) const;
	int GetCBufferIndex(LPCSTR cbuffername) const;
	int GetCBufferOffset(LPCSTR cbuffername) const;
	int GetCBufferType(LPCSTR cbuffername) const;
	int GetPropertyIndex(const LPCSTR cbufferName, const LPCSTR key) const;
	int GetPropertyOffset(const LPCSTR cbufferName, const LPCSTR key) const;
	int GetPropertyLength(const LPCSTR cbufferName, const LPCSTR key) const;
	void GetPropertyInfo(const LPCSTR cbufferName, const LPCSTR key, int&, int&, int&) const;
	void GetCBufferInfo(LPCSTR, int&, int&, int&, int&) const;
	DShaderBuffer* GetShaderBuffer();
	//DShaderParam* GetParam();

	static DShader* Create(WCHAR*, WCHAR*);

private:
	DShaderBuffer* m_shaderBuffer;
};