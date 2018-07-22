#ifdef _DGAPI_OPENGL

#include "DOpenGLSystem.h"
#include "DOpenGLCore.h"

DOpenGLSystem::DOpenGLSystem()
{
}


DOpenGLSystem::~DOpenGLSystem()
{
}

GLFWwindow * DOpenGLSystem::GetWindow()
{
	return m_window;
}

bool DOpenGLSystem::OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api)
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(screenWidth, screenHeight, "DEngine", NULL, NULL);


	if (m_window == NULL)
	{
		MessageBox(NULL, L"创建窗口失败", L"初始化失败", MB_ERR_INVALID_CHARS);
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	if (api != DGRAPHICS_API_OPENGL)
	{
		return false;
	}

	m_graphicsMgr = new DGraphics();
	if (!m_graphicsMgr->Init(screenWidth, screenHeight, fullScreen, api))
	{
		return false;
	}

	return true;
}

bool DOpenGLSystem::IsWindowShouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void DOpenGLSystem::OnShutdown()
{
}

void DOpenGLSystem::OnFrameEnd()
{
	glfwPollEvents();
}

#endif