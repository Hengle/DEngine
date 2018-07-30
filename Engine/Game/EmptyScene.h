#pragma once
#include "DScene.h"

class EmptyScene : public DScene
{
public:
	EmptyScene(SCENEID sceneId, char* sceneName);

protected:
	virtual void OnLoad();
	virtual void OnUnLoad();
	virtual void OnUpdate();

private:
	DCamera* m_camera;
	DVector3 m_lookAtPoint;
	float m_lookDistance;
};

