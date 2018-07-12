#pragma once
#include "DImGUICore.h"
#include <d3d9.h>

/*Direct3d9 ImGUI渲染与管理模块*/
class DImGUICore9 : public DImGUICore
{
public:
	DImGUICore9();
	~DImGUICore9();

	void Init(HWND hwnd, LPDIRECT3DDEVICE9 device);
	virtual void ShutDown();
	virtual void NewFrame();
	virtual void EndFrame();
	virtual void Render();

private:
	IMGUI_API bool        ImGui_ImplDX9_Init(void* hwnd, IDirect3DDevice9* device);
	IMGUI_API void        ImGui_ImplDX9_Shutdown();
	IMGUI_API void        ImGui_ImplDX9_NewFrame();
	IMGUI_API void        ImGui_ImplDX9_RenderDrawData(ImDrawData* draw_data);

	// Use if you want to reset your rendering device without losing ImGui state.
	IMGUI_API void        ImGui_ImplDX9_InvalidateDeviceObjects();
	IMGUI_API bool        ImGui_ImplDX9_CreateDeviceObjects();
	bool ImGui_ImplDX9_CreateFontsTexture();

private:
	HWND                     g_hWnd = 0;
	INT64                    g_Time = 0;
	INT64                    g_TicksPerSecond = 0;
	ImGuiMouseCursor         g_LastMouseCursor = ImGuiMouseCursor_COUNT;

	// DirectX data
	LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
	LPDIRECT3DVERTEXBUFFER9  g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9   g_pIB = NULL;
	LPDIRECT3DTEXTURE9       g_FontTexture = NULL;
	int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;
};

