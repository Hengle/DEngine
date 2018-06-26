#include "DTexture.h"
#include "DSystem.h"

DTexture::DTexture()
{
}

DTexture::~DTexture()
{
}

void DTexture::Apply(UINT)
{
}

DTexture2D::DTexture2D() : DTexture()
{
	m_textureRes = 0;
}

DTexture2D::~DTexture2D()
{
}

void DTexture2D::Destroy()
{
	if (m_textureRes != NULL)
	{
		m_textureRes->Release();
		delete m_textureRes;
		m_textureRes = NULL;
	}

	return;
}

void DTexture2D::Apply(UINT offset)
{
	DTexture::Apply(offset);
	
	m_textureRes->Apply(offset, m_wrapMode);
}

DWrapMode DTexture2D::GetWrapMode()
{
	return m_wrapMode;
}

DTexture2D * DTexture2D::Create(WCHAR *filename)
{
	DTexture2D* tex = new DTexture2D();
	tex->m_textureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateTextureRes(filename);
	tex->m_wrapMode = DWrapMode_Repeat;
	return tex;
}

DTexture2D * DTexture2D::Create(WCHAR *filename, DWrapMode wrapmode)
{
	DTexture2D* tex = new DTexture2D();
	tex->m_textureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateTextureRes(filename);
	tex->m_wrapMode = wrapmode;
	return tex;
}

DRenderTexture::DRenderTexture() : DTexture()
{
	m_renderTextureRes = 0;
}

DRenderTexture::~DRenderTexture()
{

}

void DRenderTexture::Destroy()
{
	if (m_renderTextureRes != NULL)
	{
		m_renderTextureRes->Release();
		delete m_renderTextureRes;
		m_renderTextureRes = NULL;
	}

	return;
}

void DRenderTexture::Apply(UINT offset)
{
	DTexture::Apply(offset);

	m_renderTextureRes->Apply(offset, m_wrapMode);
}

DWrapMode DRenderTexture::GetWrapMode()
{
	return m_wrapMode;
}

DRenderTextureViewRes * DRenderTexture::GetTextureRes()
{
	return m_renderTextureRes;
}

DRenderTexture * DRenderTexture::Create(float width, float height)
{
	DRenderTexture* tex = new DRenderTexture();
	tex->m_renderTextureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateRenderTextureRes(width, height);
	tex->m_wrapMode = DWrapMode_Repeat;
	return tex;
}

DRenderTexture * DRenderTexture::Create(float width, float height, DWrapMode wrapMode)
{
	DRenderTexture* tex = new DRenderTexture();
	tex->m_renderTextureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateRenderTextureRes(width, height);
	tex->m_wrapMode = wrapMode;
	return tex;
}
