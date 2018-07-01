﻿#pragma once
#include "DResObject.h"
#include "DMath.h"
#include "DColor.h"
#include "DGLCore.h"
#include "DTexture.h"

class DShaderConstantTable
{
public:
	void Release();
	void SetMatrix(const LPCSTR key, DMatrix4x4&);
	void SetVector4(const LPCSTR key, DVector4&);
	void SetVector3(const LPCSTR key, DVector3&);
	void SetVector2(const LPCSTR key, DVector2&);
	void SetColor(const LPCSTR key, DColor&);
	void SetFloat(const LPCSTR key, float);
	void SetTexture(const LPCSTR key, DTexture*);

public:
	std::map<std::string, float*> params;
	std::map<std::string, DTexture*> textures;
};

class DShader : public DResObject
{
private:
	DShader();

public:
	~DShader();

	virtual void Destroy();
	//unsigned int GetCBufferCount() const;
	//unsigned int GetPropertyCount() const;
	//void GetPropertyInfo(const LPCSTR key, DShaderParamDesc* desc) const;
	bool HasProperty(const LPCSTR key) const;
	//void ApplyParams(int cindex, int coffset, int csize, int stype, float* params) const;
	//void ApplyRes(const LPCSTR key, DTexture*) const;
	void Apply(DShaderConstantTable*);
	void Draw();
	int GetVertexUsage();
	static DShader* Create(WCHAR*, WCHAR*);
	static void ReleaseGlobalConstants();

	static void SetGlobalMatrix(const LPCSTR key, DMatrix4x4&);
	static void SetGlobalVector4(const LPCSTR key, DVector4&);
	static void SetGlobalVector3(const LPCSTR key, DVector3&);
	static void SetGlobalVector2(const LPCSTR key, DVector2&);
	static void SetGlobalColor(const LPCSTR key, DColor&);
	static void SetGlobalFloat(const LPCSTR key, float);
	static void SetGlobalTexture(const LPCSTR key, DTexture*);

private:
	DShaderRes* m_shaderRes;
};

static DShaderConstantTable* sGlobalShaderConstants;