#include "DScene.h"
#include "DSystem.h"


DScene::DScene(SCENEID sceneId, char * sceneName)
{
	m_sceneId = sceneId;
	m_sceneName = sceneName;
	m_isLoaded = false;
	m_isEnter = false;
	m_displayObjects = 0;
	m_camera = 0;
	m_light = 0;
}

DScene::~DScene()
{
	m_sceneName = 0;
	m_displayObjects->clear();
	delete m_displayObjects;
	m_displayObjects = 0;
}

void DScene::DrawGUI()
{
	if (!m_isEnter)
		return;
	if (!m_isLoaded)
		return;
	OnGUI();
}

void DScene::Render()
{
	if (!m_isEnter)
		return;
	if (!m_isLoaded)
		return;
	if (m_camera != NULL)
	{
		m_camera->Render();
		m_camera->RenderFilter();
	}
	/*if (m_camera != NULL)
		m_camera->BeginRender();
	if (m_displayObjects != NULL) {
		int i, size;
		size = m_displayObjects->size();
		for (int i = 0; i < size; i++) {
			DDisplayObject* obj = m_displayObjects->at(i);
			obj->Render();
		}
	}
	OnRender();
	if (m_camera != NULL)
	{
		m_camera->EndRender();
		m_camera->RenderFilter();
	}*/
}

void DScene::Update()
{
	if (!m_isEnter)
		return;
	if (!m_isLoaded)
		return;
	if (m_camera != NULL)
		m_camera->Update();
	if (m_light != NULL)
		m_light->Update();
	if (m_displayObjects != NULL) {
		int i, size;
		size = m_displayObjects->size();
		for (int i = 0; i < size; i++) {
			DDisplayObject* obj = m_displayObjects->at(i);
			obj->Update();
		}
	}
	OnUpdate();
}

void DScene::FixedUpdate()
{
	if (!m_isEnter)
		return;
	if (!m_isLoaded)
		return;
	if (m_camera != NULL)
		m_camera->FixedUpdate();
	if (m_light != NULL)
		m_light->FixedUpdate();
	if (m_displayObjects != NULL) {
		int i, size;
		size = m_displayObjects->size();
		for (int i = 0; i < size; i++) {
			DDisplayObject* obj = m_displayObjects->at(i);
			obj->FixedUpdate();
		}
	}
	OnFixedUpdate();
}

void DScene::Enter()
{
	if (m_isEnter)
		return;
	Load();
	if (m_camera != NULL) {
		m_camera->Init();
	}
	if (m_light != NULL) {
		m_light->Init();
	}
	if (m_displayObjects != NULL) {
		int i, size;
		size = m_displayObjects->size();
		for (int i = 0; i < size; i++) {
			DDisplayObject* obj = m_displayObjects->at(i);
			obj->Init();
		}
	}
	OnEnter();
	m_isEnter = true;
}

void DScene::Exit()
{
	if (!m_isEnter)
		return;
	Exit();
	UnLoad();
	m_isEnter = false;
}

void DScene::Load()
{
	if (m_isLoaded)
		return;
	if (m_displayObjects == NULL) {
		m_displayObjects = new std::vector<DDisplayObject*>();
	}
	OnLoad();
	m_isLoaded = true;
}

void DScene::UnLoad()
{
	if (!m_isLoaded)
		return;
	if (m_displayObjects != NULL) 
	{
		int i, size;
		size = m_displayObjects->size();
		for (int i = 0; i < size; i++) 
		{
			DDisplayObject* obj = m_displayObjects->at(i);
			obj->Destroy();
			delete obj;
		}
		m_displayObjects->clear();
	}
	if (m_camera != NULL)
	{
		m_camera->Destroy();
		delete m_camera;
		m_camera = NULL;
	}
	if (m_light != NULL)
	{
		m_light->Destroy();
		delete m_light;
		m_light = NULL;
	}
	OnUnLoad();
	m_isLoaded = false;
}

char * DScene::GetSceneName()
{
	return m_sceneName;
}

SCENEID DScene::GetSceneId()
{
	return m_sceneId;
}

bool DScene::IsSceneLoaded()
{
	return m_isLoaded;
}

bool DScene::IsEnter()
{
	return m_isEnter;
}

void DScene::Destroy()
{
	Exit();
}

void DScene::Draw(bool callOnRender, DShader* replaceShader)
{
	DScene* current = DSystem::GetSceneMgr()->GetCurrentScene();
	if (current != NULL)
		current->DrawScene(callOnRender, replaceShader);
}

void DScene::DrawShadow()
{
	DScene* current = DSystem::GetSceneMgr()->GetCurrentScene();
	if (current != NULL && current->m_light != NULL)
		current->m_light->RenderShadow();
}

void DScene::OnGUI()
{
}

void DScene::OnRender()
{
}

void DScene::OnUpdate()
{
}

void DScene::OnFixedUpdate()
{
}

void DScene::OnEnter()
{
}

void DScene::OnExit()
{
}

void DScene::OnLoad()
{
}

void DScene::OnUnLoad()
{
}

void DScene::AddDisplayObject(DDisplayObject * displayObject)
{
	if (m_displayObjects == NULL)
		return;
	m_displayObjects->push_back(displayObject);
}

void DScene::SetCamera(DCamera * camera)
{
	m_camera = camera;
}

DCamera * DScene::GetCamera()
{
	return m_camera;
}

void DScene::SetLight(DLight * light)
{
	m_light = light;
}

DLight * DScene::GetLight()
{
	return m_light;
}

void DScene::DrawScene(bool callOnRender, DShader* replaceShader)
{
	if (m_displayObjects != NULL) 
	{
		int i, size;
		size = m_displayObjects->size();
		for (int i = 0; i < size; i++) 
		{
			DDisplayObject* obj = m_displayObjects->at(i);
			obj->Render(replaceShader);
		}
	}
	if (callOnRender)
		OnRender();
}