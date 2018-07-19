#pragma once
#include "DDisplayObject.h"
#include <queue>

class DRender
{
public:
	DRender();
	~DRender();
	void PushDisplayObject(DDisplayObject*, DRenderQueue);
	void Render();
	void Release();

private:
	std::queue<DDisplayObject*> m_opaqueQueue;
	std::queue<DDisplayObject*> m_transparentQueue;
};

