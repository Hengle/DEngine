#pragma once
#ifdef _DGAPI_OPENGL

#include "DOpenGLCore.h"

class DShaderProgramOpGL : public DShaderProgram
{
public:
	DShaderProgramOpGL();
	~DShaderProgramOpGL();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

protected:
	virtual void OnDraw();
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
};

class DShaderVertexProgramOpGL : public DShaderProgramOpGL
{
public:
	DShaderVertexProgramOpGL();
	~DShaderVertexProgramOpGL();
	virtual void Release();

protected:

};

class DShaderPixelProgramOpGL : public DShaderProgramOpGL
{
public:
	DShaderPixelProgramOpGL();
	~DShaderPixelProgramOpGL();

protected:

};

#endif