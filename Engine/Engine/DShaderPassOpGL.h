#pragma once
#ifdef _DGAPI_OPENGL

#include "DShaderPass.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class DShaderProgramOpGL : public DShaderProgram
{
public:
	DShaderProgramOpGL();
	~DShaderProgramOpGL();
	void InitVertexShader(const char*);
	void InitFragmentShader(const char*);
	void InitProgram();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const;
	virtual bool HasProperty(const LPCSTR key) const;
	virtual void Release();

protected:
	virtual void OnDraw();
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);

private:
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	GLuint m_ProgramID;
};

class DShaderPassOpGL : public DShaderPass
{
public:
	DShaderPassOpGL();
	~DShaderPassOpGL();
	virtual void Release();
	virtual void Draw();
	virtual int GetVertexUsage();
	virtual bool HasProperty(LPCSTR key);
	virtual void CompileShader(std::ifstream&);
	virtual int GetShaderProgramCount();
	virtual DShaderProgram* GetShaderProgram(int index);

private:
	void CompileShaderCode(std::ifstream&);
	void CompileVertexShader(std::ifstream&);
	void CompileFragmentShader(std::ifstream&);

private:
	DShaderProgramOpGL* m_program;
};

#endif