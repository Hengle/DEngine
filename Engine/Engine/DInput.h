#pragma once
#include "DEngineDefine.h"


class DInput
{
public:
	DInput();
	~DInput();

	static void GetMousePosition(int&, int&);
	static void GetDeltaMouseMove(int&, int&);
	static bool IsMousePress(int button);
	static bool IsMouseDown(int button);
	static bool IsKeyPress(DKey keycode);
	static bool IsKeyDown(DKey keycode);
};

