#pragma once
#include <d3dx9.h>

class MyTestDraw
{
public:
	MyTestDraw();
	~MyTestDraw();
	void Init();
	void Render();

private:
	ID3DXMesh*m_mesh;
};

