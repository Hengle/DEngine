#include "DTexture.h"
#include "DSystem.h"

DTexture::DTexture()
{
}

DTexture::~DTexture()
{
}

DTexture2D::DTexture2D() : DTexture()
{
	//m_texture = 0;
}

DTexture2D::~DTexture2D()
{
}

void DTexture2D::Destroy()
{
	/*if (m_texture)
	{
		m_texture->Release();
		delete m_texture;
		m_texture = NULL;
	}*/

	return;
}

DWarpMode DTexture2D::GetWarpMode()
{
	return m_warpMode;
}

DTexture2D * DTexture2D::Create(WCHAR *filename)
{
	DTexture2D* tex = new DTexture2D();
	tex->m_textureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateTextureRes(filename);
	tex->m_warpMode = DWarpMode_Repeat;
}

DTexture2D * DTexture2D::Create(WCHAR *filename, DWarpMode warpmode)
{
	DTexture2D* tex = new DTexture2D();
	tex->m_textureRes = DSystem::GetGraphicsMgr()->GetGLCore()->CreateTextureRes(filename);
	tex->m_warpMode = warpmode;
}

//DTextureBuffer * DTexture2D::GetTextureBuffer()
//{
//	return m_texture;
//}
//
//DTexture2D * DTexture2D::Create(WCHAR * fileName)
//{
//	DTextureBuffer* buffer = DSystem::GetGraphicsMgr()->GetGLCore()->CreateTextureBuffer(fileName);
//	DTexture2D* tex = new DTexture2D();
//	tex->m_texture = buffer;
//	return tex;
//}
