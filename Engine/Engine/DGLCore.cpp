#include "DGLCore.h"

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

void DGLCore::GetResolution(FLOAT & width, FLOAT & height)
{
	width = m_width;
	height = m_height;
}