#include "stdafx.h"
#include "D3D9Core.h"


D3D9Core::D3D9Core()
{
}


D3D9Core::~D3D9Core()
{
}

bool D3D9Core::Init(int, int, bool, HWND)
{
	return false;
}

void D3D9Core::Destroy()
{
}

void D3D9Core::BeginRender(float, float, float, float)
{
}

void D3D9Core::EndRender()
{
}

DMeshBuffer * D3D9Core::CreateMeshBuffer(int vertexCount, int indexCount, int dataSize, const float * vertices, const unsigned long * indices)
{
	return nullptr;
}

DTextureBuffer * D3D9Core::CreateTextureBuffer(WCHAR * fileName)
{
	return nullptr;
}

DShaderBuffer * D3D9Core::CreateShaderBuffer(WCHAR * vertexShader, WCHAR * pixelShader)
{
	return nullptr;
}

void D3D9Core::ApplyShaderParams(DShaderBuffer * shaderBuffer, int cindex, int coffset, int csize, int stype, float * params)
{
}

void D3D9Core::ApplyTextureParams(DTextureBuffer * textureBuffer)
{
}

void D3D9Core::DrawMesh(const DMeshBuffer *, int)
{
}

void D3D9Core::DrawShader(const DShaderBuffer *, int)
{
}

void D3D9Core::SetBackBufferRenderTarget()
{
}

void DMeshBuffer9::Release()
{
}

void DTextureBuffer9::Release()
{
}

unsigned int DShaderBuffer9::GetCBufferCount() const
{
	return 0;
}

unsigned int DShaderBuffer9::GetPropertyCount() const
{
	return 0;
}

void DShaderBuffer9::GetPropertyInfo(const LPCSTR key, int & cindex, int & coffset, int & clength, int & poffset, int & plength, int & stype) const
{
}

bool DShaderBuffer9::HasProperty(const LPCSTR key) const
{
	return false;
}

void DShaderBuffer9::Release()
{
}
