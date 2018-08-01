#pragma once



class DInput
{
public:
	DInput();
	~DInput();

	static void GetMousePosition(int&, int&);
	static void GetDeltaMouseMove(int&, int&);
	static bool IsMousePress(int button);
	static bool IsMouseDown(int button);
	static bool IsKeyPress(int keycode);
	static bool IsKeyDown(int keycode);
};

