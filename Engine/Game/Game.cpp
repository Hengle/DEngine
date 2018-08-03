// Game.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameFrame.h"

#include "glad\glad.h"
#include "GLFW\glfw3.h"
//#include "DMath.h"
//#include <Windows.h>
//#include <D3DX10math.h>
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int nCmdShow)
{
	GameFrame* frame = new GameFrame();
	frame->Create(1024, 768, false, DGRAPHICS_API_D3D9);
	frame->Destroy();
	
	
	return 0;
}