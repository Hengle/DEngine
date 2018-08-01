#pragma once
#include "DColor.h"
#include "DGeometry.h"
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
	void PostProcess(DMatrix4x4&, DMatrix4x4&);
	
private:
	void ProcessDraw(DMatrix4x4&, DMatrix4x4&);

private:
	float* m_vertices;
	unsigned long* m_indices;
	//DGeometryBufferDesc m_geoDesc;
	DColor m_currentColor;
	DGeometryRes* m_geometryRes;
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
	void GlVertex(DVector3*);
	void GlVertex3(float, float, float);
	void GlColor(DColor*);
	void GlPushMatrix();
	void GlPopMatrix();
	void GlLoadIdentity();
	void GlLoadProjectionMatrix(DMatrix4x4&);
	void GlLoadOrtho();
	void GlMultiMatrix(DMatrix4x4&);
	void Release();
	void GetModelView(DMatrix4x4&);
	void GetProjection(DMatrix4x4&);

private:
	void GenerateNewProcesses();

private:
	DMatrix4x4 m_currentMV;
	DMatrix4x4 m_currentP;
	unsigned int m_currentPLen, m_prePLen;
	/*modelview矩阵栈*/
	std::stack<DMatrix4x4> m_MVMatrixStack;
	/*投影矩阵栈*/
	std::stack<DMatrix4x4> m_PMatrixStack;
	std::vector<DGLDrawerProcess*> m_processVector;

	DGLDrawerProcess* m_currentProcess;
};

