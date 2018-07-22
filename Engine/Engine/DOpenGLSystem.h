#pragma once
#ifdef _DGAPI_OPENGL

#include "DSystem.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class DOpenGLSystem : public DSystem
{
public:
	DOpenGLSystem();
	~DOpenGLSystem();

protected:
	virtual bool OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api);
	virtual void OnShutdown();

private:
	GLFWwindow* m_window;
};
#endif
