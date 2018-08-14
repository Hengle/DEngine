#include "DInput.h"
#include "DSystem.h"

DInput::DInput()
{
}


DInput::~DInput()
{
}

void DInput::GetMousePosition(int & x, int & y)
{
	IInputCore* core = DSystem::GetInputMgr();
	if (core != NULL)
	{
		core->GetMousePosition(x, y);
	}
}

void DInput::GetDeltaMouseMove(int & x, int & y)
{
	IInputCore* core = DSystem::GetInputMgr();
	if (core != NULL)
	{
		core->GetDeltaMouseMove(x, y);
	}
}

bool DInput::IsMousePress(int button)
{
	IInputCore* core = DSystem::GetInputMgr();
	if (core != NULL)
	{
		return core->IsMousePress(button);
	}
	return false;
}

bool DInput::IsMouseDown(int button)
{
	IInputCore* core = DSystem::GetInputMgr();
	if (core != NULL)
	{
		return core->IsMouseDown(button);
	}
	return false;
}

bool DInput::IsKeyPress(DKey keycode)
{
	IInputCore* core = DSystem::GetInputMgr();
	if (core != NULL)
	{
		return core->IsKeyPress(keycode);
	}
	return false;
}

bool DInput::IsKeyDown(DKey keycode)
{
	IInputCore* core = DSystem::GetInputMgr();
	if (core != NULL)
	{
		return core->IsKeyDown(keycode);
	}
	return false;
}

//bool DInput::ReadKeyboard()
//{
//	HRESULT result;
//
//
//	// Read the keyboard device.
//	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
//	if (FAILED(result))
//	{
//		// If the keyboard lost focus or was not acquired then try to get control back.
//		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
//		{
//			m_keyboard->Acquire();
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//
//bool DInput::ReadMouse(HWND hwnd)
//{
//	HRESULT result;
//
//	POINT point;
//	GetCursorPos(&point);
//	ScreenToClient(hwnd, &point);
//
//	m_mouseX = (int)(point.x);
//	m_mouseY = (int)(point.y);
//
//	if (m_mouseX < 0) { m_mouseX = 0; }
//	if (m_mouseY < 0) { m_mouseY = 0; }
//
//	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
//	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
//
//	// Read the mouse device.
//	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
//	if (FAILED(result))
//	{
//		// If the mouse lost focus or was not acquired then try to get control back.
//		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
//		{
//			m_mouse->Acquire();
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//
//void DInput::ProcessInput()
//{
//	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
//	m_deltaMouseX = m_mouseState.lX;
//	m_deltaMouseY = m_mouseState.lY;
//
//	// Ensure the mouse location doesn't exceed the screen width or height.
//
//	return;
//}
