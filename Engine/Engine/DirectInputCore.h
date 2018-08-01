#pragma once
#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11

#include "DInputCore.h"
#include <dinput.h>

class DirectInputCore : IInputCore
{
public:
	DirectInputCore();
	~DirectInputCore();

	bool Init(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool InputLoop();

	virtual void GetMousePosition(int&, int&);
	virtual void GetDeltaMouseMove(int&, int&);
	virtual bool IsMousePress(int button);
	virtual bool IsMouseDown(int button);
	virtual bool IsKeyPress(int keycode);
	virtual bool IsKeyDown(int keycode);

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
	HWND m_hwnd;
};

#endif