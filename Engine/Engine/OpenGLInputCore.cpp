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

bool OpenGLInputCore::IsKeyPress(int keycode)
{
	return glfwGetKey(m_window, keycode) == GLFW_PRESS;
}

bool OpenGLInputCore::IsKeyDown(int keycode)
{
	if (glfwGetKey(m_window, keycode) == GLFW_PRESS && m_keyboardDownState[keycode] == 0)
	{
		m_keyboardDownState[keycode] = 1;
	}
	else if (glfwGetKey(m_window, keycode) == GLFW_PRESS && m_keyboardDownState[keycode] == 1)
	{
		m_keyboardDownState[keycode] = 2;
	}
	else if (glfwGetKey(m_window, keycode) != GLFW_PRESS)
	{
		m_keyboardDownState[keycode] = 0;
	}

	return m_keyboardDownState[keycode] == 1;
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

#endif