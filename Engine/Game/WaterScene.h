#pragma once
#include "DScene.h"
#include "DDisplayObject.h"
#include "DShader.h"
#include "WaterFilter.h"

class WaterScene : public DScene
{
public:
	WaterScene(SCENEID sceneId, char* sceneName);
	~WaterScene();

protected:
	virtual void OnGUI();
	virtual void OnLoad();
	virtual void OnUnLoad();
	virtual void OnUpdate();

private:
	DCamera* m_camera;
	DVector3 m_lookAtPoint;
	float m_lookDistance;

	WaterFilter* m_filter;
};

