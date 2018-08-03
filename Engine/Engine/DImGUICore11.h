#pragma once
#ifdef _DGAPI_D3D11

#include "D3DImGUICore.h"
#include <D3D11.h>

/*Direct3d11 ImGUI渲染与管理模块*/
class DImGUICore11 : public D3DImGUICore
{
public:
	DImGUICore11();
	~DImGUICore11();

	void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual void ShutDown();
	virtual void NewFrame();
	virtual void EndFrame();
	virtual void Render();

private:
	IMGUI_API bool        ImGui_ImplDX11_Init(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	IMGUI_API void        ImGui_ImplDX11_Shutdown();
	IMGUI_API void        ImGui_ImplDX11_NewFrame();
	IMGUI_API void        ImGui_ImplDX11_RenderDrawData(ImDrawData* draw_data);

	IMGUI_API void        ImGui_ImplDX11_InvalidateDeviceObjects();
	IMGUI_API bool        ImGui_ImplDX11_CreateDeviceObjects();
	void ImGui_ImplDX11_CreateFontsTexture();

private:
	HWND                     g_hWnd = 0;
	INT64                    g_Time = 0;
	INT64                    g_TicksPerSecond = 0;
	ImGuiMouseCursor         g_LastMouseCursor = ImGuiMouseCursor_COUNT;
	// DirectX data
	ID3D11Device*            g_pd3dDevice = NULL;
	ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
	ID3D11Buffer*            g_pVB = NULL;
	ID3D11Buffer*            g_pIB = NULL;
	ID3D10Blob *             g_pVertexShaderBlob = NULL;
	ID3D11VertexShader*      g_pVertexShader = NULL;
	ID3D11InputLayout*       g_pInputLayout = NULL;
	ID3D11Buffer*            g_pVertexConstantBuffer = NULL;
	ID3D10Blob *             g_pPixelShaderBlob = NULL;
	ID3D11PixelShader*       g_pPixelShader = NULL;
	ID3D11SamplerState*      g_pFontSampler = NULL;
	ID3D11ShaderResourceView*g_pFontTextureView = NULL;
	ID3D11RasterizerState*   g_pRasterizerState = NULL;
	ID3D11BlendState*        g_pBlendState = NULL;
	ID3D11DepthStencilState* g_pDepthStencilState = NULL;
	int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;
};

#endif