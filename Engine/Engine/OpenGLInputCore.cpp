#if _DGAPI_OPENGL
#include "OpenGLInputCore.h"


OpenGLInputCore::OpenGLInputCore()
{
	m_window = 0;
}


OpenGLInputCore::~OpenGLInputCore()
{
}

void OpenGLInputCore::Init(GLFWwindow * window)
{
	m_window = window;

	InitKeyCodes();
}

void OpenGLInputCore::GetMousePosition(int & x, int & y)
{
	x = (int)m_mousePosX;
	y = (int)m_mousePosY;
}

void OpenGLInputCore::GetDeltaMouseMove(int & x, int & y)
{
	x = (int)m_deltaMousePosX;
	y = (int)m_deltaMousePosY;
}

bool OpenGLInputCore::IsMousePress(int button)
{
	return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

bool OpenGLInputCore::IsMouseDown(int button)
{
	if (glfwGetMouseButton(m_window, button) == GLFW_PRESS && m_mouseBtnState[button] == 0)
	{
		m_mouseBtnState[button] = 1;
	}
	else if (glfwGetMouseButton(m_window, button) == GLFW_PRESS && m_mouseBtnState[button] == 1)
	{
		m_mouseBtnState[button] = 2;
	}
	else if (glfwGetMouseButton(m_window, button) != GLFW_PRESS)
	{
		m_mouseBtnState[button] = 0;
	}

	return m_mouseBtnState[button] == 1;
}

bool OpenGLInputCore::IsKeyPress(DKey keycode)
{
	if (keycode < 0 || keycode >= 102)
		return false;
	unsigned int k = m_keyCodes[keycode];
	return glfwGetKey(m_window, k) == GLFW_PRESS;
}

bool OpenGLInputCore::IsKeyDown(DKey keycode)
{
	if (keycode < 0 || keycode >= 102)
		return false;
	unsigned int k = m_keyCodes[keycode];
	if (glfwGetKey(m_window, k) == GLFW_PRESS && m_keyboardDownState[k] == 0)
	{
		m_keyboardDownState[k] = 1;
	}
	else if (glfwGetKey(m_window, k) == GLFW_PRESS && m_keyboardDownState[k] == 1)
	{
		m_keyboardDownState[k] = 2;
	}
	else if (glfwGetKey(m_window, k) != GLFW_PRESS)
	{
		m_keyboardDownState[k] = 0;
	}

	return m_keyboardDownState[k] == 1;
}

void OpenGLInputCore::Update()
{
	double posX, posY;
	glfwGetCursorPos(m_window, &posX, &posY);
	m_deltaMousePosX = posX - m_mousePosX;
	m_deltaMousePosY = posY - m_mousePosY;
	m_mousePosX = posX;
	m_mousePosY = posY;
}

void OpenGLInputCore::InitKeyCodes()
{
	m_keyCodes[DKey_Escape] = GLFW_KEY_ESCAPE;
	m_keyCodes[DKey_1] = GLFW_KEY_1;
	m_keyCodes[DKey_2] = GLFW_KEY_2;
	m_keyCodes[DKey_3] = GLFW_KEY_3;
	m_keyCodes[DKey_4] = GLFW_KEY_4;
	m_keyCodes[DKey_5] = GLFW_KEY_5;
	m_keyCodes[DKey_6] = GLFW_KEY_6;
	m_keyCodes[DKey_7] = GLFW_KEY_7;
	m_keyCodes[DKey_8] = GLFW_KEY_8;
	m_keyCodes[DKey_9] = GLFW_KEY_9;
	m_keyCodes[DKey_0] = GLFW_KEY_0;
	m_keyCodes[DKey_Minus] = GLFW_KEY_MINUS;
	m_keyCodes[DKey_Equals] = GLFW_KEY_EQUAL;
	m_keyCodes[DKey_Back] = GLFW_KEY_BACKSPACE;
	m_keyCodes[DKey_Tab] = GLFW_KEY_TAB;
	m_keyCodes[DKey_Delete] = GLFW_KEY_DELETE;
	m_keyCodes[DKey_Q] = GLFW_KEY_Q;
	m_keyCodes[DKey_W] = GLFW_KEY_W;
	m_keyCodes[DKey_E] = GLFW_KEY_E;
	m_keyCodes[DKey_R] = GLFW_KEY_R;
	m_keyCodes[DKey_T] = GLFW_KEY_T;
	m_keyCodes[DKey_Y] = GLFW_KEY_Y;
	m_keyCodes[DKey_U] = GLFW_KEY_U;
	m_keyCodes[DKey_I] = GLFW_KEY_I;
	m_keyCodes[DKey_O] = GLFW_KEY_O;
	m_keyCodes[DKey_P] = GLFW_KEY_P;
	m_keyCodes[DKey_A] = GLFW_KEY_A;
	m_keyCodes[DKey_S] = GLFW_KEY_S;
	m_keyCodes[DKey_D] = GLFW_KEY_D;
	m_keyCodes[DKey_F] = GLFW_KEY_F;
	m_keyCodes[DKey_G] = GLFW_KEY_G;
	m_keyCodes[DKey_H] = GLFW_KEY_H;
	m_keyCodes[DKey_J] = GLFW_KEY_J;
	m_keyCodes[DKey_K] = GLFW_KEY_K;
	m_keyCodes[DKey_L] = GLFW_KEY_L;
	m_keyCodes[DKey_Z] = GLFW_KEY_Z;
	m_keyCodes[DKey_X] = GLFW_KEY_X;
	m_keyCodes[DKey_C] = GLFW_KEY_C;
	m_keyCodes[DKey_V] = GLFW_KEY_V;
	m_keyCodes[DKey_B] = GLFW_KEY_B;
	m_keyCodes[DKey_N] = GLFW_KEY_N;
	m_keyCodes[DKey_M] = GLFW_KEY_M;
	m_keyCodes[DKey_LeftBracket] = GLFW_KEY_LEFT_BRACKET;
	m_keyCodes[DKey_RightBracket] = GLFW_KEY_RIGHT_BRACKET;
	m_keyCodes[DKey_Return] = GLFW_KEY_ENTER;
	m_keyCodes[DKey_LeftControl] = GLFW_KEY_LEFT_CONTROL;
	m_keyCodes[DKey_RightControl] = GLFW_KEY_RIGHT_CONTROL;
	m_keyCodes[DKey_Semicolon] = GLFW_KEY_SEMICOLON;
	m_keyCodes[DKey_Apostrophe] = GLFW_KEY_APOSTROPHE;
	m_keyCodes[DKey_Grave] = GLFW_KEY_GRAVE_ACCENT;
	m_keyCodes[DKey_LeftShift] = GLFW_KEY_LEFT_SHIFT;
	m_keyCodes[DKey_RightShift] = GLFW_KEY_RIGHT_SHIFT;
	m_keyCodes[DKey_Backslash] = GLFW_KEY_BACKSLASH;
	m_keyCodes[DKey_Comma] = GLFW_KEY_COMMA;
	m_keyCodes[DKey_Period] = GLFW_KEY_PERIOD;
	m_keyCodes[DKey_Slash] = GLFW_KEY_SLASH;
	m_keyCodes[DKey_Multiply] = GLFW_KEY_KP_MULTIPLY;
	m_keyCodes[DKey_LeftAlt] = GLFW_KEY_LEFT_ALT;
	m_keyCodes[DKey_RightAlt] = GLFW_KEY_RIGHT_ALT;
	m_keyCodes[DKey_Space] = GLFW_KEY_SPACE;
	m_keyCodes[DKey_Capital] = GLFW_KEY_CAPS_LOCK;
	m_keyCodes[DKey_F1] = GLFW_KEY_F1;
	m_keyCodes[DKey_F2] = GLFW_KEY_F2;
	m_keyCodes[DKey_F3] = GLFW_KEY_F3;
	m_keyCodes[DKey_F4] = GLFW_KEY_F4;
	m_keyCodes[DKey_F5] = GLFW_KEY_F5;
	m_keyCodes[DKey_F6] = GLFW_KEY_F6;
	m_keyCodes[DKey_F7] = GLFW_KEY_F7;
	m_keyCodes[DKey_F8] = GLFW_KEY_F8;
	m_keyCodes[DKey_F9] = GLFW_KEY_F9;
	m_keyCodes[DKey_F10] = GLFW_KEY_F10;
	m_keyCodes[DKey_F11] = GLFW_KEY_F11;
	m_keyCodes[DKey_F12] = GLFW_KEY_F12;
	m_keyCodes[DKey_NumLock] = GLFW_KEY_NUM_LOCK;
	m_keyCodes[DKey_Scroll] = GLFW_KEY_SCROLL_LOCK;
	m_keyCodes[DKey_Num0] = GLFW_KEY_KP_0;
	m_keyCodes[DKey_Num1] = GLFW_KEY_KP_1;
	m_keyCodes[DKey_Num2] = GLFW_KEY_KP_2;
	m_keyCodes[DKey_Num3] = GLFW_KEY_KP_3;
	m_keyCodes[DKey_Num4] = GLFW_KEY_KP_4;
	m_keyCodes[DKey_Num5] = GLFW_KEY_KP_5;
	m_keyCodes[DKey_Num6] = GLFW_KEY_KP_6;
	m_keyCodes[DKey_Num7] = GLFW_KEY_KP_7;
	m_keyCodes[DKey_Num8] = GLFW_KEY_KP_8;
	m_keyCodes[DKey_Num9] = GLFW_KEY_KP_9;
	m_keyCodes[DKey_Add] = GLFW_KEY_KP_ADD;
	m_keyCodes[DKey_Sub] = GLFW_KEY_KP_SUBTRACT;
	m_keyCodes[DKey_Decimal] = GLFW_KEY_KP_DECIMAL;
	m_keyCodes[DKey_NumEnter] = GLFW_KEY_KP_ENTER;
	m_keyCodes[DKey_Divide] = GLFW_KEY_KP_DIVIDE;
	m_keyCodes[DKey_Pause] = GLFW_KEY_PAUSE;
	m_keyCodes[DKey_Home] = GLFW_KEY_HOME;
	m_keyCodes[DKey_Left] = GLFW_KEY_LEFT;
	m_keyCodes[DKey_Up] = GLFW_KEY_UP;
	m_keyCodes[DKey_Down] = GLFW_KEY_DOWN;
	m_keyCodes[DKey_Right] = GLFW_KEY_RIGHT;
	m_keyCodes[DKey_PageUp] = GLFW_KEY_PAGE_UP;
	m_keyCodes[DKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	m_keyCodes[DKey_End] = GLFW_KEY_END;
	m_keyCodes[DKey_Insert] = GLFW_KEY_INSERT;
}

#endif