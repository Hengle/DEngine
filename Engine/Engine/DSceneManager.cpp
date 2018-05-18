#include "DSceneManager.h"


DSceneManager::DSceneManager()
{
	m_currentScene = 0;
	m_sceneMap = new std::map<SCENEID, DScene*>();
}


DSceneManager::~DSceneManager()
{
	if (m_currentScene != NULL) {
		m_currentScene = NULL;
	}
	if (m_sceneMap != NULL) {
		delete m_sceneMap;
		m_sceneMap = NULL;
	}
}

void DSceneManager::RegisterScene(DScene * scene)
{
	if (scene == NULL)
		return;
	if (m_sceneMap->count(scene->GetSceneId()) == 0) {
		m_sceneMap->insert(std::pair<SCENEID, DScene*>(scene->GetSceneId(), scene));
	}
}

DScene * DSceneManager::ChangeScene(SCENEID sceneId)
{
	if (m_sceneMap->count(sceneId) == 0) {
		return NULL;
	}
	if (m_currentScene != NULL) {
		m_currentScene->Exit();
		m_currentScene = NULL;
	}
	DScene* scene = m_sceneMap->at(sceneId);
	if (scene != NULL) {
		scene->Enter();
		m_currentScene = scene;
		return m_currentScene;
	}
	return NULL;
}

DScene * DSceneManager::LoadScene(SCENEID sceneId)
{
	if (m_sceneMap->count(sceneId) == 0) {
		return NULL;
	}
	DScene* scene = m_sceneMap->at(sceneId);
	if (scene != NULL) {
		scene->Load();
		return scene;
	}
}

DScene * DSceneManager::GetCurrentScene()
{
	return m_currentScene;
}

void DSceneManager::DrawGUI()
{
	if (m_currentScene != NULL) {
		m_currentScene->DrawGUI();
	}
}

void DSceneManager::RenderScene(ID3D11DeviceContext* deviceContext)
{
	if (m_currentScene != NULL) {
		m_currentScene->Render(deviceContext);
	}
}

void DSceneManager::UpdateScene()
{
	if (m_currentScene != NULL) {
		m_currentScene->Update();
	}
}

void DSceneManager::FixedUpdateScene()
{
	if (m_currentScene != NULL) {
		m_currentScene->FixedUpdate();
	}
}

bool DSceneManager::UnLoadScene(SCENEID sceneId)
{
	if (m_sceneMap->count(sceneId) == 0) {
		return false;
	}
	DScene* scene = m_sceneMap->at(sceneId);
	if (scene != NULL) {
		scene->UnLoad();
		return true;
	}
	return false;
}

void DSceneManager::UnloadAllScene()
{
	if (m_sceneMap != NULL) {
		std::map<SCENEID, DScene*>::iterator  iter;
		for (iter = m_sceneMap->begin(); iter != m_sceneMap->end(); iter++)
		{
			if (iter->second != NULL)
			{
				iter->second->UnLoad();
				delete iter->second;
			}
		}
		m_sceneMap->clear();
	}
}
