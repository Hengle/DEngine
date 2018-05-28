#pragma once
#include "DSceneObject.h"

class DDisplayObject : public DSceneObject
{
public:
	DDisplayObject();
	~DDisplayObject();
	virtual void Render();
	virtual void Init();
	virtual void Destroy();

protected:
};
