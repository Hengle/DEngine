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

void DTexture2D::Apply(UINT offset, int index)
{
	DTexture::Apply(offset, index);
	
	m_textureRes->Apply(offset, index);
	m_textureRes->ApplyWrapMode(offset, m_wrapMode);
}

DWrapMode DTexture2D::GetWrapMode()
{
	return m_wrapMode;
}

void DTexture2D::SetWrapMode(DWrapMode wrapMode)
{
	m_wrapMode = wrapMode;
}

ITextureRes * DTexture2D::GetTextureRes()
{
	return m_textureRes;
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

DTextureCube::DTextureCube() : DTexture()
{
	m_textureRes = 0;
}

DTextureCube::~DTextureCube()
{
}

void DTextureCube::Destroy()
{
	if (m_textureRes != NULL)
	{
		m_textureRes->Release();
		delete m_textureRes;
		m_textureRes = NULL;
	}
}

void DTextureCube::Apply(UINT offset, int index)
{
	DTexture::Apply(offset, index);

	m_textureRes->Apply(offset, index);
}

DTextureCube * DTextureCube::Create(DTexture2D * right, DTexture2D * left, DTexture2D * top, DTexture2D * bottom, DTexture2D * front, DTexture2D * back)
{
	DTextureCube* tex = new DTextureCube();
	tex->m_textureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateCubeMapRes(right->GetTextureRes(), left->GetTextureRes(), top->GetTextureRes(), bottom->GetTextureRes(), front->GetTextureRes(), back->GetTextureRes());
	return tex;
}

DRenderTexture::DRenderTexture(float width, float height) : DTexture()
{
	m_renderTextureRes = 0;
	m_width = width;
	m_height = height;
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

void DRenderTexture::Apply(UINT offset, int index)
{
	DTexture::Apply(offset, index);

	m_renderTextureRes->Apply(offset, index);
	m_renderTextureRes->ApplyWrapMode(offset, m_wrapMode);
}

DWrapMode DRenderTexture::GetWrapMode()
{
	return m_wrapMode;
}

void DRenderTexture::SetWrapMode(DWrapMode wrapMode)
{
	m_wrapMode = wrapMode;
}

IRenderTextureViewRes * DRenderTexture::GetTextureRes()
{
	return m_renderTextureRes;
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
	tex->m_renderTextureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateRenderTextureRes(width, height);
	tex->m_wrapMode = DWrapMode_Repeat;
	return tex;
}

DRenderTexture * DRenderTexture::Create(float width, float height, DWrapMode wrapMode)
{
	DRenderTexture* tex = new DRenderTexture(width, height);
	tex->m_renderTextureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateRenderTextureRes(width, height);
	tex->m_wrapMode = wrapMode;
	return tex;
}

