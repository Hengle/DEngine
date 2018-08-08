#ifdef _DGAPI_D3D9
#include "DTextureRes9.h"
#include "DSystem.h"

DTextureRes9::DTextureRes9(LPDIRECT3DDEVICE9 device, WCHAR * filename)
{
	m_device = device;

	HRESULT result = D3DXCreateTextureFromFile(device, filename, &m_texture);
	m_isSuccess = false;
	if (FAILED(result))
	{
		return;
	}
	m_isSuccess = true;
}

DTextureRes9::DTextureRes9(LPDIRECT3DDEVICE9 device)
{
	/*m_device = device;

	HRESULT result = device->CreateTexture(64, 64, 1, D3DUSAGE_DYNAMIC, D3DFMT_R8G8B8, D3DPOOL_MANAGED, &m_texture, NULL);

	m_isSuccess = false;
	if (FAILED(result))
	{
		return;
	}
	m_isSuccess = true;

	D3DLOCKED_RECT rect;
	RECT r;
	r.left = 0;
	r.right = 64;
	r.top = 0;
	r.bottom = 64;
	m_texture->LockRect(0, &rect, &r, );*/
}

DTextureRes9::~DTextureRes9()
{
}

void DTextureRes9::Apply(UINT textureOffset, int)
{
	if (m_isSuccess)
	{
		m_device->SetTexture(textureOffset, m_texture);
	}
}

void DTextureRes9::ApplyWrapMode(UINT textureOffset, DWrapMode mode)
{
	if (m_isSuccess)
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplySamplerState(textureOffset, mode);
	}
}

void DTextureRes9::Release()
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}
	m_device = NULL;
}

DCubeTextureRes9::DCubeTextureRes9(LPDIRECT3DDEVICE9 device, DTextureRes9 * right, DTextureRes9 * left, DTextureRes9 * top, DTextureRes9 * bottom, DTextureRes9 * front, DTextureRes9 * back)
{
	LPDIRECT3DCUBETEXTURE9 cubemap;
	m_isSuccess = false;

	m_device = device;

	/*HRESULT result = device->CreateCubeTexture()
	if (FAILED(result))
	{
		return;
	}
	cubemap->*/

	m_isSuccess = true;
}

DRenderTextureViewRes9::DRenderTextureViewRes9(LPDIRECT3DDEVICE9 device, float width, float height)
{
	HRESULT hr;
	m_isSuccess = false;
	if (FAILED(hr = D3DXCreateTexture(device, width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_texture))) {
		if (FAILED(hr = D3DXCreateTexture(device, width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_texture))) {
			return;
		}
	}
	D3DSURFACE_DESC desc;
	hr = m_texture->GetSurfaceLevel(0, &m_surface);
	if (FAILED(hr))
		return;
	hr = m_surface->GetDesc(&desc);
	if (FAILED(hr))
		return;
	if (FAILED(hr = D3DXCreateRenderToSurface(device, desc.Width, desc.Height, desc.Format, TRUE, D3DFMT_D16, &m_interface)))
		return;

	m_device = device;
	m_isSuccess = true;
}

DRenderTextureViewRes9::~DRenderTextureViewRes9()
{
}

void DRenderTextureViewRes9::Apply(UINT textureOffset, int)
{
	if (m_isSuccess)
	{
		m_device->SetTexture(textureOffset, m_texture);
	}
}

void DRenderTextureViewRes9::ApplyWrapMode(UINT textureOffset, DWrapMode mode)
{
	if (m_isSuccess)
	{
		DSystem::GetGraphicsMgr()->GetGLCore()->ApplySamplerState(textureOffset, mode);
	}
}

void DRenderTextureViewRes9::Release()
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}
	if (m_surface != NULL)
	{
		m_surface->Release();
		m_surface = NULL;
	}
	if (m_interface != NULL)
	{
		m_interface->Release();
		m_interface = NULL;
	}
	m_device = NULL;
}

IRenderBuffer * DRenderTextureViewRes9::GetColorBuffer()
{
	return nullptr;
}

IRenderBuffer * DRenderTextureViewRes9::GetDepthBuffer()
{
	return nullptr;
}

#endif
