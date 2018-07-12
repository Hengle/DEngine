#pragma once
#include "DScene.h"
#include <map>

/*����������*/
class DSceneManager
{
public:
	DSceneManager();
	~DSceneManager();
	/*ע�᳡��*/
	void RegisterScene(DScene*);
	/*�л�����*/
	DScene* ChangeScene(SCENEID);
	/*���س���-����Ԥ�ȼ��س���*/
	DScene* LoadScene(SCENEID);
	/*��ȡ��ǰ����*/
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
