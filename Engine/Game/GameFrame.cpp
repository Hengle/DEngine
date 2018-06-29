﻿#include "stdafx.h"
#include "GameFrame.h"
#include "TestScene.h"
#include "GlTestScene.h"

GameFrame::GameFrame()
{
}


GameFrame::~GameFrame()
{
}

void GameFrame::OnInit()
{
	RegisterScene(new TestScene(5, "tst"));
	RegisterScene(new GlTestScene(6, "glsce"));
	ChangeScene(5);
}
