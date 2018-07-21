#pragma once
#include "DDisplayObject.h"
#include <queue>

class DRenderer
{
public:
	DRenderer();
	~DRenderer();
	void PushDisplayObject(DDisplayObject*, DRenderQueue);
	void Render();
	void Clear();
	void Release();

private:
	std::queue<DDisplayObject*> m_opaqueQueue;
	std::queue<DDisplayObject*> m_transparentQueue;
};

