#pragma once
#include "DColor.h"
#include "DMesh.h"
#include "DMaterial.h"
#include <stack>

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
	void Release();

private:
	void Draw();

private:
	float* m_vertices;
	unsigned long* m_indices;

	DColor m_currentColor;
	DMatrix4x4 m_currentMatrix;
	DMeshRes* m_meshRes;
	DShader* m_shader;
	DMaterial* m_material;

	unsigned long m_currentIndex, m_preIndex;
	std::stack<DMatrix4x4> m_matrixStack;
	bool m_hasDrawCommand;
};

