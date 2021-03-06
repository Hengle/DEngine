﻿#pragma once
#include "DResObject.h"
#include "DMath.h"
#include "DColor.h"
#include "DTexture.h"
#include "DShaderBlock.h"
#include "DShaderConstant.h"

/*shader常量表*/
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

/*shader类*/
class DShader : public DResObject
{
private:
	DShader();

public:
	~DShader();

	virtual void Destroy();
	bool HasProperty(const LPCSTR key) const;
	void ApplyStates(int);
	void ApplyParams(DShaderConstantTable*, int);
	int GetVertexUsage(int);
	int GetPassCount();
	DRenderQueue GetRenderQueue();
	bool IsPassEnable(int);
	void SetPassEnable(int, bool);
	DShaderPass* GetPass(int);
	static DShader* Create(char*);
	static void ReleaseGlobalConstants();

	static void SetGlobalMatrix(const LPCSTR key, DMatrix4x4&);
	static void SetGlobalVector4(const LPCSTR key, DVector4&);
	static void SetGlobalVector3(const LPCSTR key, DVector3&);
	static void SetGlobalVector2(const LPCSTR key, DVector2&);
	static void SetGlobalColor(const LPCSTR key, DColor&);
	static void SetGlobalFloat(const LPCSTR key, float);
	static void SetGlobalTexture(const LPCSTR key, DTexture*);

private:
	void ApplyShaderProgramParams(DShaderProgram * program, DShaderConstantTable * constantTable);

private:
	DShaderBlock* m_shaderBlock;
};

/*全局shader常量*/
static DShaderConstantTable* sGlobalShaderConstants;