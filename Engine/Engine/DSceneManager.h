#pragma once
#include "DScene.h"
#include <map>

/*场景管理器*/
class DSceneManager
{
public:
	DSceneManager();
	~DSceneManager();
	/*注册场景*/
	void RegisterScene(DScene*);
	/*切换场景*/
	DScene* ChangeScene(SCENEID);
	/*加载场景-用于预先加载场景*/
	DScene* LoadScene(SCENEID);
	/*获取当前场景*/
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
