#ifdef _DGAPI_OPENGL

#include "DOpenGLSystem.h"


DOpenGLSystem::DOpenGLSystem()
{
}


DOpenGLSystem::~DOpenGLSystem()
{
}

bool DOpenGLSystem::OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api)
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(screenWidth, screenHeight, "DEngine", NULL, NULL);



	return false;
}

void DOpenGLSystem::OnShutdown()
{
}

#endif