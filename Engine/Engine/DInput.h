#pragma once
#include <dinput.h>

class DInput
{
public:
	DInput();
	~DInput();
	bool Init(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool InputLoop(HWND);

	static void GetMousePosition(int&, int&);
	static void GetDeltaMouseMove(int&, int&);
	static bool IsMousePress(int button);
	static bool IsMouseDown(int button);
	static bool IsKeyPress(int keycode);
	static bool IsKeyDown(int keycode);

private:
	bool ReadKeyboard();
	bool ReadMouse(HWND);
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	unsigned char m_keyboardDownState[256];
	DIMOUSESTATE m_mouseState;
	BYTE m_mouseBtnState[4];

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	int m_deltaMouseX, m_deltaMouseY;
};

