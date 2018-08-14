#include "DTexture.h"
#include "DSystem.h"

DTexture::DTexture()
{
}

DTexture::~DTexture()
{
}

void DTexture::Apply(UINT, int)
{
}

DTexture2D::DTexture2D() : DTexture()
{
	m_textureWrapper = 0;
}

DTexture2D::~DTexture2D()
{
}

void DTexture2D::Destroy()
{
	if (m_textureWrapper != NULL)
	{
		m_textureWrapper->Release();
		delete m_textureWrapper;
		m_textureWrapper = NULL;
	}

	return;
}

void DTexture2D::Apply(UINT offset, int index)
{
	DTexture::Apply(offset, index);
	
	m_textureWrapper->Apply(offset, index);
	m_textureWrapper->ApplyWrapMode(offset, m_wrapMode);
}

DWrapMode DTexture2D::GetWrapMode()
{
	return m_wrapMode;
}

void DTexture2D::SetWrapMode(DWrapMode wrapMode)
{
	m_wrapMode = wrapMode;
}

ITextureWrapper * DTexture2D::GetTextureWrapper()
{
	return m_textureWrapper;
}

DTexture2D * DTexture2D::Create(WCHAR *filename)
{
	DTexture2D* tex = new DTexture2D();
	tex->m_textureWrapper = DSystem::GetGraphicsMgr()->GetGLCore()->CreateTextureWrapper(filename);
	tex->m_wrapMode = DWrapMode_Repeat;
	return tex;
}

DTexture2D * DTexture2D::Create(WCHAR *filename, DWrapMode wrapmode)
{
	DTexture2D* tex = new DTexture2D();
	tex->m_textureWrapper = DSystem::GetGraphicsMgr()->GetGLCore()->CreateTextureWrapper(filename);
	tex->m_wrapMode = wrapmode;
	return tex;
}

DTextureCube::DTextureCube() : DTexture()
{
	m_textureWrapper = 0;
}

DTextureCube::~DTextureCube()
{
}

void DTextureCube::Destroy()
{
	if (m_textureWrapper != NULL)
	{
		m_textureWrapper->Release();
		delete m_textureWrapper;
		m_textureWrapper = NULL;
	}
}

void DTextureCube::Apply(UINT offset, int index)
{
	DTexture::Apply(offset, index);

	m_textureWrapper->Apply(offset, index);
}

DTextureCube * DTextureCube::Create(DTexture2D * right, DTexture2D * left, DTexture2D * top, DTexture2D * bottom, DTexture2D * front, DTexture2D * back)
{
	DTextureCube* tex = new DTextureCube();
	tex->m_textureWrapper = DSystem::GetGraphicsMgr()->GetGLCore()->CreateCubeMapWrapper(right->GetTextureWrapper(), left->GetTextureWrapper(), top->GetTextureWrapper(), bottom->GetTextureWrapper(), front->GetTextureWrapper(), back->GetTextureWrapper());
	return tex;
}

DRenderTexture::DRenderTexture(float width, float height) : DTexture()
{
	m_renderTextureWrapper = 0;
	m_width = width;
	m_height = height;
}

DRenderTexture::~DRenderTexture()
{

}

void DRenderTexture::Destroy()
{
	if (m_renderTextureWrapper != NULL)
	{
		m_renderTextureWrapper->Release();
		delete m_renderTextureWrapper;
		m_renderTextureWrapper = NULL;
	}

	return;
}

void DRenderTexture::Apply(UINT offset, int index)
{
	DTexture::Apply(offset, index);

	m_renderTextureWrapper->Apply(offset, index);
	m_renderTextureWrapper->ApplyWrapMode(offset, m_wrapMode);
}

DWrapMode DRenderTexture::GetWrapMode()
{
	return m_wrapMode;
}

void DRenderTexture::SetWrapMode(DWrapMode wrapMode)
{
	m_wrapMode = wrapMode;
}

IRenderTextureViewWrapper * DRenderTexture::GetTextureWrapper()
{
	return m_renderTextureWrapper;
}

float DRenderTexture::GetWidth() const
{
	return m_width;
}

float DRenderTexture::GetHeight() const
{
	return m_height;
}

DRenderTexture * DRenderTexture::Create(float width, float height)
{
	DRenderTexture* tex = new DRenderTexture(width, height);
	tex->m_renderTextureWrapper = DSystem::GetGraphicsMgr()->GetGLCore()->CreateRenderTextureWrapper(width, height);
	tex->m_wrapMode = DWrapMode_Clamp;
	return tex;
}

DRenderTexture * DRenderTexture::Create(float width, float height, DWrapMode wrapMode)
{
	DRenderTexture* tex = new DRenderTexture(width, height);
	tex->m_renderTextureWrapper = DSystem::GetGraphicsMgr()->GetGLCore()->CreateRenderTextureWrapper(width, height);
	tex->m_wrapMode = wrapMode;
	return tex;
}

