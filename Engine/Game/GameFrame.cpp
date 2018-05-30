#include "stdafx.h"
#include "GameFrame.h"
#include "TestScene.h"


GameFrame::GameFrame()
{
}


GameFrame::~GameFrame()
{
}

void GameFrame::OnInit()
{
	RegisterScene(new TestScene(5, "tst"));
	ChangeScene(5);
}
