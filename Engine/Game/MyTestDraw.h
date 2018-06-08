#pragma once
#include <d3dx9.h>
#include "MyTestShader.h"

class MyTestDraw
{
public:
	MyTestDraw();
	~MyTestDraw();
	void Init();
	void Render();
	void Release();

private:
	ID3DXMesh*m_mesh;
	MyTestShader* m_shader;
};

