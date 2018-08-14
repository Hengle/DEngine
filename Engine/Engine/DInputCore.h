#pragma once
#include "DEngineDefine.h"

interface IInputCore
{
	virtual void GetMousePosition(int&, int&) = 0;
	virtual void GetDeltaMouseMove(int&, int&) = 0;
	virtual bool IsMousePress(int button) = 0;
	virtual bool IsMouseDown(int button) = 0;
	virtual bool IsKeyPress(DKey keycode) = 0;
	virtual bool IsKeyDown(DKey keycode) = 0;
};

