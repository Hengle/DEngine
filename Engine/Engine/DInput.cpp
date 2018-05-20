#include "DInput.h"


DInput::DInput()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}


DInput::~DInput()
{
}

bool DInput::Init(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
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
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

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
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void DInput::Shutdown()
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

	return;
}

bool DInput::InputLoop(HWND hwnd)
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse(hwnd);
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

void DInput::GetMousePosition(int & x, int & y)
{
	x = DInput::GetInstance()->m_mouseX;
	y = DInput::GetInstance()->m_mouseY;
}

void DInput::GetDeltaMouseMove(int & x, int & y)
{
	x = DInput::GetInstance()->m_deltaMouseX;
	y = DInput::GetInstance()->m_deltaMouseY;
}

bool DInput::IsMousePress(int button)
{
	return DInput::GetInstance()->m_mouseState.rgbButtons[button] & 0x80;
}

bool DInput::IsMouseDown(int button)
{
	if (DInput::GetInstance()->m_mouseState.rgbButtons[button] & 0x80 && DInput::GetInstance()->m_mouseBtnState[button] == 0)
	{
		DInput::GetInstance()->m_mouseBtnState[button] = 1;
	}
	else if (DInput::GetInstance()->m_mouseState.rgbButtons[button] & 0x80 && DInput::GetInstance()->m_mouseBtnState[button] == 1)
	{
		DInput::GetInstance()->m_mouseBtnState[button] = 2;
	}
	else if (!(DInput::GetInstance()->m_mouseState.rgbButtons[button] & 0x80))
	{
		DInput::GetInstance()->m_mouseBtnState[button] = 0;
	}

	return DInput::GetInstance()->m_mouseBtnState[button] == 1;
}

bool DInput::IsKeyPress(int keycode)
{
	return DInput::GetInstance()->m_keyboardState[keycode] & 0x80;
}

bool DInput::IsKeyDown(int keycode)
{
	if (DInput::GetInstance()->m_keyboardState[keycode] & 0x80 && DInput::GetInstance()->m_keyboardDownState[keycode] == 0)
	{
		DInput::GetInstance()->m_keyboardDownState[keycode] = 1;
	}
	else if (DInput::GetInstance()->m_keyboardState[keycode] & 0x80 && DInput::GetInstance()->m_keyboardDownState[keycode] == 1)
	{
		DInput::GetInstance()->m_keyboardDownState[keycode] = 2;
	}
	else if (!(DInput::GetInstance()->m_keyboardState[keycode] & 0x80))
	{
		DInput::GetInstance()->m_keyboardDownState[keycode] = 0;
	}

	return DInput::GetInstance()->m_keyboardDownState[keycode] == 1;
}

bool DInput::ReadKeyboard()
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

bool DInput::ReadMouse(HWND hwnd)
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

void DInput::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_deltaMouseX = m_mouseState.lX;
	m_deltaMouseY = m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.

	return;
}
