#include "DGLCore.h"

DMeshRes::DMeshRes()
{
	m_isInitialized = false;
}

void DMeshRes::Init(DMeshBufferDesc * desc)
{
	m_isInitialized = OnInit(desc);
}

void DMeshRes::Draw(DMeshTopology topology)
{
	if (m_isInitialized)
		OnDraw(topology);
}

bool DMeshRes::IsInitialized()
{
	return m_isInitialized;
}

DShaderRes::DShaderRes()
{
	m_isInitialized = false;
	m_cbufferCount = 0;
	m_propertyCount = 0;
}

unsigned int DShaderRes::GetCBufferCount() const
{
	return m_cbufferCount;
}

unsigned int DShaderRes::GetPropertyCount() const
{
	return m_propertyCount;
}

void DShaderRes::Init(WCHAR * vsfile, WCHAR * psfile)
{
	m_isInitialized = OnInit(vsfile, psfile);
}

void DShaderRes::ApplyParams(int cindex, int coffset, int csize, int stype, float * params)
{
	if (m_isInitialized)
		OnApplyParams(cindex, coffset, csize, stype, params);
}

void DShaderRes::Draw()
{
	if (m_isInitialized)
		OnDraw();
}

bool DShaderRes::IsInitialized()
{
	return m_isInitialized;
}


DGLCore::DGLCore()
{
}


DGLCore::~DGLCore()
{
}

bool DGLCore::Init(int width, int height, bool, HWND)
{
	m_width = width;
	m_height = height;
	return true;
}

void DGLCore::GetResolution(float & width, float & height)
{
	width = m_width;
	height = m_height;
}