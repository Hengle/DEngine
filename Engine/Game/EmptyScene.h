#pragma once
#include "DScene.h"
#include "TestFilter.h"

class EmptyScene : public DScene
{
public:
	EmptyScene(SCENEID sceneId, char* sceneName);

protected:
	virtual void OnLoad();
	virtual void OnUnLoad();
	virtual void OnUpdate();
	virtual void OnRender();

private:
	DCamera* m_camera;
	DVector3 m_lookAtPoint;
	float m_lookDistance;
	TestFilter* m_filter;
	DRenderTexture* m_rt;
	//DMaterial* testmat;
	//DTexture2D* testtex;
};

