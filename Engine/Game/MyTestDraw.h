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
	IDirect3DVertexBuffer9*m_mesh;
	MyTestShader* m_shader;
};

