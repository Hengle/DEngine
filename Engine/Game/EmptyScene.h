#pragma once
#include "DScene.h"

class EmptyScene : public DScene
{
public:
	EmptyScene(SCENEID sceneId, char* sceneName);

protected:
	virtual void OnLoad();
};

