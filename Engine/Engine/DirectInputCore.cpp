#if _DGAPI_D3D9 || _DGAPI_D3D10 || _DGAPI_D3D11
#include "DirectInputCore.h"

DirectInputCore::DirectInputCore()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}


DirectInputCore::~DirectInputCore()
{
}
bool DirectInputCore::Init(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	/*result = m_keyboard->Acquire();
	if (FAILED(result))
	{
	return false;
	}*/

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	/*result = m_mouse->Acquire();
	if (FAILED(result))
	{
	return false;
	}*/
	m_hwnd = hwnd;

	InitKeyCodes();

	return true;
}
void DirectInputCore::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	m_hwnd = 0;

	return;
}
bool DirectInputCore::InputLoop()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse(m_hwnd);
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}
void DirectInputCore::GetMousePosition(int & x, int & y)
{
	x = m_mouseX;
	y = m_mouseY;
}

void DirectInputCore::GetDeltaMouseMove(int & x, int & y)
{
	x = m_deltaMouseX;
	y = m_deltaMouseY;
}

bool DirectInputCore::IsMousePress(int button)
{
	return m_mouseState.rgbButtons[button] & 0x80;
}

bool DirectInputCore::IsMouseDown(int button)
{
	if (m_mouseState.rgbButtons[button] & 0x80 && m_mouseBtnState[button] == 0)
	{
		m_mouseBtnState[button] = 1;
	}
	else if (m_mouseState.rgbButtons[button] & 0x80 && m_mouseBtnState[button] == 1)
	{
		m_mouseBtnState[button] = 2;
	}
	else if (!(m_mouseState.rgbButtons[button] & 0x80))
	{
		m_mouseBtnState[button] = 0;
	}

	return m_mouseBtnState[button] == 1;
}

bool DirectInputCore::IsKeyPress(DKey keycode)
{
	if (keycode < 0 || keycode >= 102)
		return false;
	unsigned int k = m_keyCodes[keycode];
	return m_keyboardState[k] & 0x80;
}

bool DirectInputCore::IsKeyDown(DKey keycode)
{
	if (keycode < 0 || keycode >= 102)
		return false;
	unsigned int k = m_keyCodes[keycode];
	if (m_keyboardState[k] & 0x80 && m_keyboardDownState[k] == 0)
	{
		m_keyboardDownState[k] = 1;
	}
	else if (m_keyboardState[k] & 0x80 && m_keyboardDownState[k] == 1)
	{
		m_keyboardDownState[k] = 2;
	}
	else if (!(m_keyboardState[k] & 0x80))
	{
		m_keyboardDownState[k] = 0;
	}

	return m_keyboardDownState[k] == 1;
}

void DirectInputCore::InitKeyCodes()
{
	m_keyCodes[DKey_Escape] = DIK_ESCAPE;
	m_keyCodes[DKey_1] = DIK_1;
	m_keyCodes[DKey_2] = DIK_2;
	m_keyCodes[DKey_3] = DIK_3;
	m_keyCodes[DKey_4] = DIK_4;
	m_keyCodes[DKey_5] = DIK_5;
	m_keyCodes[DKey_6] = DIK_6;
	m_keyCodes[DKey_7] = DIK_7;
	m_keyCodes[DKey_8] = DIK_8;
	m_keyCodes[DKey_9] = DIK_9;
	m_keyCodes[DKey_0] = DIK_0;
	m_keyCodes[DKey_Minus] = DIK_MINUS;
	m_keyCodes[DKey_Equals] = DIK_EQUALS;
	m_keyCodes[DKey_Back] = DIK_BACK;
	m_keyCodes[DKey_Tab] = DIK_TAB;
	m_keyCodes[DKey_Delete] = DIK_DELETE;
	m_keyCodes[DKey_Q] = DIK_Q;
	m_keyCodes[DKey_W] = DIK_W;
	m_keyCodes[DKey_E] = DIK_E;
	m_keyCodes[DKey_R] = DIK_R;
	m_keyCodes[DKey_T] = DIK_T;
	m_keyCodes[DKey_Y] = DIK_Y;
	m_keyCodes[DKey_U] = DIK_U;
	m_keyCodes[DKey_I] = DIK_I;
	m_keyCodes[DKey_O] = DIK_O;
	m_keyCodes[DKey_P] = DIK_P;
	m_keyCodes[DKey_A] = DIK_A;
	m_keyCodes[DKey_S] = DIK_S;
	m_keyCodes[DKey_D] = DIK_D;
	m_keyCodes[DKey_F] = DIK_F;
	m_keyCodes[DKey_G] = DIK_G;
	m_keyCodes[DKey_H] = DIK_H;
	m_keyCodes[DKey_J] = DIK_J;
	m_keyCodes[DKey_K] = DIK_K;
	m_keyCodes[DKey_L] = DIK_L;
	m_keyCodes[DKey_Z] = DIK_Z;
	m_keyCodes[DKey_X] = DIK_X;
	m_keyCodes[DKey_C] = DIK_C;
	m_keyCodes[DKey_V] = DIK_V;
	m_keyCodes[DKey_B] = DIK_B;
	m_keyCodes[DKey_N] = DIK_N;
	m_keyCodes[DKey_M] = DIK_M;
	m_keyCodes[DKey_LeftBracket] = DIK_LBRACKET;
	m_keyCodes[DKey_RightBracket] = DIK_RBRACKET;
	m_keyCodes[DKey_Return] = DIK_RETURN;
	m_keyCodes[DKey_LeftControl] = DIK_LCONTROL;
	m_keyCodes[DKey_RightControl] = DIK_RCONTROL;
	m_keyCodes[DKey_Semicolon] = DIK_SEMICOLON;
	m_keyCodes[DKey_Apostrophe] = DIK_APOSTROPHE;
	m_keyCodes[DKey_Grave] = DIK_GRAVE;
	m_keyCodes[DKey_LeftShift] = DIK_LSHIFT;
	m_keyCodes[DKey_RightShift] = DIK_RSHIFT;
	m_keyCodes[DKey_Backslash] = DIK_BACKSLASH;
	m_keyCodes[DKey_Comma] = DIK_COMMA;
	m_keyCodes[DKey_Period] = DIK_PERIOD;
	m_keyCodes[DKey_Slash] = DIK_SLASH;
	m_keyCodes[DKey_Multiply] = DIK_MULTIPLY;
	m_keyCodes[DKey_LeftAlt] = DIK_LMENU;
	m_keyCodes[DKey_RightAlt] = DIK_RMENU;
	m_keyCodes[DKey_Space] = DIK_SPACE;
	m_keyCodes[DKey_Capital] = DIK_CAPITAL;
	m_keyCodes[DKey_F1] = DIK_F1;
	m_keyCodes[DKey_F2] = DIK_F2;
	m_keyCodes[DKey_F3] = DIK_F3;
	m_keyCodes[DKey_F4] = DIK_F4;
	m_keyCodes[DKey_F5] = DIK_F5;
	m_keyCodes[DKey_F6] = DIK_F6;
	m_keyCodes[DKey_F7] = DIK_F7;
	m_keyCodes[DKey_F8] = DIK_F8;
	m_keyCodes[DKey_F9] = DIK_F9;
	m_keyCodes[DKey_F10] = DIK_F10;
	m_keyCodes[DKey_F11] = DIK_F11;
	m_keyCodes[DKey_F12] = DIK_F12;
	m_keyCodes[DKey_NumLock] = DIK_NUMLOCK;
	m_keyCodes[DKey_Scroll] = DIK_SCROLL;
	m_keyCodes[DKey_Num0] = DIK_NUMPAD0;
	m_keyCodes[DKey_Num1] = DIK_NUMPAD1;
	m_keyCodes[DKey_Num2] = DIK_NUMPAD2;
	m_keyCodes[DKey_Num3] = DIK_NUMPAD3;
	m_keyCodes[DKey_Num4] = DIK_NUMPAD4;
	m_keyCodes[DKey_Num5] = DIK_NUMPAD5;
	m_keyCodes[DKey_Num6] = DIK_NUMPAD6;
	m_keyCodes[DKey_Num7] = DIK_NUMPAD7;
	m_keyCodes[DKey_Num8] = DIK_NUMPAD8;
	m_keyCodes[DKey_Num9] = DIK_NUMPAD9;
	m_keyCodes[DKey_Add] = DIK_ADD;
	m_keyCodes[DKey_Sub] = DIK_SUBTRACT;
	m_keyCodes[DKey_Decimal] = DIK_DECIMAL;
	m_keyCodes[DKey_NumEnter] = DIK_NUMPADENTER;
	m_keyCodes[DKey_Divide] = DIK_DIVIDE;
	m_keyCodes[DKey_Pause] = DIK_PAUSE;
	m_keyCodes[DKey_Home] = DIK_HOME;
	m_keyCodes[DKey_Left] = DIK_LEFT;
	m_keyCodes[DKey_Up] = DIK_UP;
	m_keyCodes[DKey_Down] = DIK_DOWN;
	m_keyCodes[DKey_Right] = DIK_RIGHT;
	m_keyCodes[DKey_PageUp] = DIK_PRIOR;
	m_keyCodes[DKey_PageDown] = DIK_NEXT;
	m_keyCodes[DKey_End] = DIK_END;
	m_keyCodes[DKey_Insert] = DIK_INSERT;

}

bool DirectInputCore::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool DirectInputCore::ReadMouse(HWND hwnd)
{
	HRESULT result;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hwnd, &point);

	m_mouseX = (int)(point.x);
	m_mouseY = (int)(point.y);

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void DirectInputCore::ProcessInput()
{
	m_deltaMouseX = m_mouseState.lX;
	m_deltaMouseY = m_mouseState.lY;
}
#endif