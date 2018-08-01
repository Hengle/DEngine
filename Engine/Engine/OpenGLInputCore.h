#pragma once
#if _DGAPI_OPENGL

#include "DInputCore.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class OpenGLInputCore : IInputCore
{
public:
	OpenGLInputCore();
	~OpenGLInputCore();

	void Init(GLFWwindow*);

	virtual void GetMousePosition(int&, int&);
	virtual void GetDeltaMouseMove(int&, int&);
	virtual bool IsMousePress(int button);
	virtual bool IsMouseDown(int button);
	virtual bool IsKeyPress(int keycode);
	virtual bool IsKeyDown(int keycode);

	void Update();

private:
	GLFWwindow* m_window;
	double m_mousePosX, m_mousePosY, m_deltaMousePosX, m_deltaMousePosY;

	unsigned char m_keyboardDownState[256];
	BYTE m_mouseBtnState[4];
};

#endif