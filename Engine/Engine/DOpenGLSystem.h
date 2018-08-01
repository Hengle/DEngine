#pragma once
#ifdef _DGAPI_OPENGL

#include "DSystem.h"
#include "OpenGLInputCore.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class DOpenGLSystem : public DSystem
{
public:
	DOpenGLSystem();
	~DOpenGLSystem();
	GLFWwindow* GetWindow();

protected:
	virtual bool OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api);
	virtual bool IsWindowShouldClose();
	virtual void OnShutdown();
	virtual void OnFrameBegin();
	virtual void OnFrameEnd();
	virtual IInputCore* GetInputCore();

private:
	GLFWwindow* m_window;
	OpenGLInputCore* m_inputCore;
};
#endif
