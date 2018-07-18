#include "DRenderer.h"


DRenderer::DRenderer()
{
}


DRenderer::~DRenderer()
{
}

void DRenderer::PushDisplayObject(DDisplayObject * displayObject, DRenderQueue queue)
{
	if (queue == DRenderQueue_Opaque)
		m_opaqueQueue.push(displayObject);
	else if (queue == DRenderQueue_Transparent)
		m_transparentQueue.push(displayObject);
}

void DRenderer::Render()
{
	while (!m_opaqueQueue.empty())
	{
		DDisplayObject* obj = m_opaqueQueue.front();
		m_opaqueQueue.pop();
		obj->RenderObject();
	}
	while (!m_transparentQueue.empty())
	{
		DDisplayObject* obj = m_transparentQueue.front();
		m_transparentQueue.pop();
		obj->RenderObject();
	}
}

void DRenderer::ShutDown()
{
	while (!m_opaqueQueue.empty())
	{
		m_opaqueQueue.pop();
	}
	while (!m_transparentQueue.empty())
	{
		m_transparentQueue.pop();
	}
}

