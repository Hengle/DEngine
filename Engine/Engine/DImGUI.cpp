#include "DImGUI.h"


DImGUI::DImGUI()
{
}


DImGUI::~DImGUI()
{
}

void DImGUI::Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChinese());
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplDX11_Init(hwnd, device, deviceContext);

	// Setup style
	ImGui::StyleColorsDark();
}

void DImGUI::ShutDown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void DImGUI::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
}

void DImGUI::BeginWindow(const char* windowName)
{
	ImGui::Begin(windowName);
}

void DImGUI::EndWindow()
{
	ImGui::End();
}

void DImGUI::Label(const char * text)
{
}

//void DImGUI::DrawGUI(DSceneManager* sceneManager)
//{
//	//ImGui::SetCursorPosX(20);
//	//ImGui::SetCursorPosY(80);
//	//ImGui::Button("xxxa");
//
//
//	//{
//	//	static float f = 0.0f;
//	//	static int counter = 0;
//	//	ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
//	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
//	//	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//	//	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
//	//	//ImGui::Checkbox("Another Window", &show_another_window);
//
//	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
//	//		counter++;
//	//	ImGui::SameLine();
//	//	ImGui::Text("counter = %d", counter);
//
//	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//	//}
//
//	//// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
//	////if (show_another_window)
//	//{
//	//	/*ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
//	//	ImGui::Begin("Another Window", &show_another_window);
//	//	ImGui::Text("Hello from another window!");
//	//	if (ImGui::Button("Close Me"))
//	//		show_another_window = false;
//	//	ImGui::End();*/
//	//}
//}

void DImGUI::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DImGUI::GUIMessageHandler(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wparam, lparam))
		return true;
	return false;
}
