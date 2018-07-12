#pragma once
#include "DScene.h"
#include "DDisplayObject.h"
#include "DShader.h"
#include "DMesh.h"

class GlTestScene : public DScene
{
public:
	GlTestScene(SCENEID sceneId, char* sceneName);

protected:
	virtual void OnGUI();
	virtual void OnLoad();
	virtual void OnUnLoad();
	virtual void OnRender();
	virtual void OnUpdate();

private:
	//DShader* m_testcolorshader;
	DMaterial* m_testcolormat;
	DCamera* cam;
};

