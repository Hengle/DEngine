#pragma once
#include "DScene.h"
#include <map>

class DSceneManager
{
public:
	DSceneManager();
	~DSceneManager();
	void RegisterScene(DScene*);
	DScene* ChangeScene(SCENEID);
	DScene* LoadScene(SCENEID);
	DScene* GetCurrentScene();
	void DrawGUI();
	void RenderScene();
	void UpdateScene();
	void FixedUpdateScene();
	bool UnLoadScene(SCENEID);
	void UnloadAllScene();

private:
	DScene *m_currentScene;
	std::map<SCENEID, DScene*>* m_sceneMap;
};
