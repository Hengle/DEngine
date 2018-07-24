#pragma once
#ifdef _DGAPI_OPENGL

#include "DOpenGLCore.h"

/*
Geometry��ԴOpenGL�ײ�
*/
class DGeometryResOpGL : public DGeometryRes
{
public:
	DGeometryResOpGL(int vertexUsage /*�����÷�����*/, bool dynamic /*�Ƿ�Ϊ��̬mesh*/);
	~DGeometryResOpGL();
	virtual void Release();

protected:
	virtual void OnRefresh(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual bool OnInit(float* vertexbuffer, unsigned long* indexbuffer, int vertexCount, int indexCount);
	virtual void OnDraw(DGeometryTopology);
};

#endif