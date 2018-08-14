﻿#include "stdafx.h"
#include "GameFrame.h"
#include "TestScene.h"
#include "GlTestScene.h"
#include "EmptyScene.h"
#include "WaterScene.h"

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
	RegisterScene(new EmptyScene(7, "empty"));
	RegisterScene(new WaterScene(8, "water"));
	ChangeScene(8);
}
