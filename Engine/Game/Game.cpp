// Game.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GameFrame.h"
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int nCmdShow)
{
	GameFrame* frame = new GameFrame();
	frame->Create();
	frame->Destroy();


	return 0;
}

//int main() {
//
//	DObjModelLoader::LoadObj("as.obj");
//
//	/*char* k = "1/2/3";
//	int id0, id1, id2;
//	sscanf_s(k, "%d/%d/%d", &id0, &id1, &id2);
//
//	cout << id0 << endl;
//	cout << id1 << endl;
//	cout << id2 << endl;*/
//
//	int c = 0;
//	cin >> c;
//
//	return 0;
//}