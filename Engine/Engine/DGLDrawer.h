#pragma once
#include "DColor.h"
#include "DMesh.h"
#include "DMaterial.h"
#include <stack>
#include <vector>

class DGLDrawerProcess
{
public:
	DGLDrawerProcess();
	void Release();
	void ProcessVector(float, float, float);
	void ProcessColor(DColor*);
	void PostProcess(DMaterial*, DMatrix4x4&, DMatrix4x4&);
	
private:
	void ProcessDraw(DMaterial*, DMatrix4x4&, DMatrix4x4&);

private:
	float* m_vertices;
	unsigned long* m_indices;
	DColor m_currentColor;
	DMeshRes* m_meshRes;
	unsigned long m_currentIndex, m_preIndex;
	bool m_hasDrawCommand;
};

class DGLDrawer
{
public:
	DGLDrawer();
	~DGLDrawer();

	void PostGL();
	void GlBegin();
	void GlEnd();
	void GlVector3(DVector3*);
	void GlVector(float, float, float);
	void GlColor(DColor*);
	void GlPushMatrix();
	void GlPopMatrix();
	void GlLoadIdentity();
	void GlLoadProjectionMatrix(DMatrix4x4&);
	void GlLoadOrtho();
	void GlMultiMatrix(DMatrix4x4&);
	//void GlSetMaterial(DMaterial*);
	void Release();

private:
	void GenerateNewProcesses();

private:
	//float* m_vertices;
	//unsigned long* m_indices;

	//DColor m_currentColor;
	DMatrix4x4 m_currentMV;
	DMatrix4x4 m_currentP;
	//DMeshRes* m_meshRes;
	//DShader* m_shader;
	//DMaterial* m_material;

	//unsigned long m_currentIndex, m_preIndex;
	unsigned int m_currentPLen, m_prePLen;
	std::stack<DMatrix4x4> m_MVMatrixStack;
	std::stack<DMatrix4x4> m_PMatrixStack;
	std::vector<DGLDrawerProcess*> m_processVector;
	//bool m_hasDrawCommand;

	DGLDrawerProcess* m_currentProcess;
};

