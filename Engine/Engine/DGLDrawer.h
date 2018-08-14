#pragma once
#include "DColor.h"
#include "DGeometry.h"
#include "DMaterial.h"
#include <stack>
#include <vector>

class DGLDrawer
{
public:
	DGLDrawer();
	~DGLDrawer();

	//void PostGL();
	void GlBegin(DGeometryTopology mode);
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

//private:
//	void GenerateNewProcesses();

private:
	DMatrix4x4 m_currentMV;
	DMatrix4x4 m_currentP;
	//unsigned int m_currentPLen, m_prePLen;
	/*modelview矩阵栈*/
	std::stack<DMatrix4x4> m_MVMatrixStack;
	/*投影矩阵栈*/
	std::stack<DMatrix4x4> m_PMatrixStack;
	DGeometryWrapper* m_geometry;
	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indecies;
	int m_vertexCount;
	int m_indexCount;
	DColor m_currentColor;
	DGeometryTopology m_mode;
	//std::vector<DGLDrawerProcess*> m_processVector;

	//DGLDrawerProcess* m_currentProcess;
};

