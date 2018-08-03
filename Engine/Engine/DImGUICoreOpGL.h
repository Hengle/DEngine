#pragma once
#ifdef _DGAPI_OPENGL

#include "DImGUICore.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class DImGUICoreOpGL : public IImGUICore
{
public:
	DImGUICoreOpGL();
	~DImGUICoreOpGL();

	void Init(GLFWwindow* window, bool install_callbacks, const char* glsl_version = NULL);
	virtual void ShutDown();
	virtual void NewFrame();
	virtual void EndFrame();
	virtual void Render();
	virtual LRESULT CALLBACK GUIMessageHandler(HWND, UINT, WPARAM, LPARAM) { return false; }

private:
	IMGUI_API bool        ImGui_ImplGlfwGL3_Init(GLFWwindow* window, bool install_callbacks, const char* glsl_version = NULL);
	IMGUI_API void        ImGui_ImplGlfwGL3_Shutdown();
	IMGUI_API void        ImGui_ImplGlfwGL3_NewFrame();
	IMGUI_API void        ImGui_ImplGlfwGL3_RenderDrawData(ImDrawData* draw_data);

	// Use if you want to reset your rendering device without losing ImGui state.
	IMGUI_API void        ImGui_ImplGlfwGL3_InvalidateDeviceObjects();
	IMGUI_API bool        ImGui_ImplGlfwGL3_CreateDeviceObjects();
};

// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
// Provided here if you want to chain callbacks.
// You can also handle inputs yourself and use those as a reference.
IMGUI_API void        ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
IMGUI_API void        ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
IMGUI_API void        ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
IMGUI_API void        ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);

#endif