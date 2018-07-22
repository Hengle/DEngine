#pragma once
#ifdef _DGAPI_D3D10

#include "DImGUICore.h"
#include <d3d10_1.h>
#include <d3d10.h>

/*Direct3d10 ImGUI渲染与管理模块*/
class DImGUICore10 : public DImGUICore
{
public:
	DImGUICore10();
	~DImGUICore10();

	void Init(HWND hwnd, ID3D10Device* device);
	virtual void ShutDown();
	virtual void NewFrame();
	virtual void EndFrame();
	virtual void Render();

private:
	IMGUI_API bool        ImGui_ImplDX10_Init(void* hwnd, ID3D10Device* device);
	IMGUI_API void        ImGui_ImplDX10_Shutdown();
	IMGUI_API void        ImGui_ImplDX10_NewFrame();
	IMGUI_API void        ImGui_ImplDX10_RenderDrawData(ImDrawData* draw_data);

	// Use if you want to reset your rendering device without losing ImGui state.
	IMGUI_API void        ImGui_ImplDX10_InvalidateDeviceObjects();
	IMGUI_API bool        ImGui_ImplDX10_CreateDeviceObjects();
	void ImGui_ImplDX10_CreateFontsTexture();

private:
	// Win32 Data
	HWND                     g_hWnd = 0;
	INT64                    g_Time = 0;
	INT64                    g_TicksPerSecond = 0;
	ImGuiMouseCursor         g_LastMouseCursor = ImGuiMouseCursor_COUNT;

	// DirectX data
	ID3D10Device*            g_pd3dDevice = NULL;
	ID3D10Buffer*            g_pVB = NULL;
	ID3D10Buffer*            g_pIB = NULL;
	ID3D10Blob *             g_pVertexShaderBlob = NULL;
	ID3D10VertexShader*      g_pVertexShader = NULL;
	ID3D10InputLayout*       g_pInputLayout = NULL;
	ID3D10Buffer*            g_pVertexConstantBuffer = NULL;
	ID3D10Blob *             g_pPixelShaderBlob = NULL;
	ID3D10PixelShader*       g_pPixelShader = NULL;
	ID3D10SamplerState*      g_pFontSampler = NULL;
	ID3D10ShaderResourceView*g_pFontTextureView = NULL;
	ID3D10RasterizerState*   g_pRasterizerState = NULL;
	ID3D10BlendState*        g_pBlendState = NULL;
	ID3D10DepthStencilState* g_pDepthStencilState = NULL;
	int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;
};

#endif