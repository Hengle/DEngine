// Game.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameFrame.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int nCmdShow)
{
	GameFrame* frame = new GameFrame();
	frame->Create();
	frame->Destroy();
	return 0;
}
