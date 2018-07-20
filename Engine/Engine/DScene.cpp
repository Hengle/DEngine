#include "DScene.h"
#include "DSystem.h"


DScene::DScene(SCENEID sceneId, char * sceneName)
{
	m_sceneId = sceneId;
	m_sceneName = sceneName;
	m_isLoaded = false;
	m_isEnter = false;
	//m_displayObjects = 0;
	//m_camera = 0;
	//m_light = 0;
	//m_transforms = 0;
	m_rootTransform = 0;
}

DScene::~DScene()
{
	m_sceneName = 0;
	/*m_transforms->clear();
	delete m_transforms;
	m_transforms = 0;*/
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
	/*if (m_camera != NULL)
	{
		m_camera->Render();
		m_camera->RenderFilter();
	}*/
	DCameraNode* node = m_cameraNode;
	while (node != NULL)
	{
		node->camera->Render();
		node = node->next;
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
	//if (m_camera != NULL)
	//	m_camera->Update();
	//if (m_light != NULL)
	//	m_light->Update();
	/*if (m_transforms != NULL) {
		int i, size;
		size = m_transforms->size();
		for (int i = 0; i < size; i++) {
			DTransform* trans = m_transforms->at(i);
			DSceneObject* sceneObj = trans->GetSceneObject();
			sceneObj->Update();
		}
	}*/
	if (m_rootTransform != NULL)
		UpdateSceneObject(m_rootTransform);
	

	OnUpdate();
}

void DScene::FixedUpdate()
{
	if (!m_isEnter)
		return;
	if (!m_isLoaded)
		return;
	//if (m_camera != NULL)
	//	m_camera->FixedUpdate();
	//if (m_light != NULL)
	//	m_light->FixedUpdate();
	/*if (m_transforms != NULL) {
		int i, size;
		size = m_transforms->size();
		for (int i = 0; i < size; i++) {
			DTransform* trans = m_transforms->at(i);
			DSceneObject* sceneObj = trans->GetSceneObject();
			sceneObj->FixedUpdate();
		}
	}*/
	if(m_rootTransform != NULL)
		FixedUpdateSceneObject(m_rootTransform);

	OnFixedUpdate();
}

void DScene::Enter()
{
	if (m_isEnter)
		return;
	Load();
	//if (m_camera != NULL) {
	//	m_camera->Init();
	//}
	//if (m_light != NULL) {
	//	m_light->Init();
	//}
	/*if (m_displayObjects != NULL) {
		int i, size;
		size = m_displayObjects->size();
		for (int i = 0; i < size; i++) {
			DDisplayObject* obj = m_displayObjects->at(i);
			obj->Init();
		}
	}*/
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
	/*if (m_transforms == NULL) {
		m_transforms = new std::vector<DTransform*>();
	}*/
	if (m_rootTransform == NULL)
	{
		m_rootTransform = new DTransform(true);
	}
	OnLoad();
	m_isLoaded = true;
}

void DScene::UnLoad()
{
	if (!m_isLoaded)
		return;
	/*if (m_transforms != NULL)
	{
		int i, size;
		size = m_transforms->size();
		for (int i = 0; i < size; i++) 
		{
			DTransform* trans = m_transforms->at(i);
			DSceneObject* obj = trans->GetSceneObject();
			obj->Destroy();
			delete obj;
		}
		m_transforms->clear();
	}*/
	if (m_rootTransform != NULL)
	{
		UnLoadSceneObject(m_rootTransform);
		m_rootTransform->Release();
		delete m_rootTransform;
		m_rootTransform = NULL;
	}
	//if (m_camera != NULL)
	//{
	//	m_camera->Destroy();
	//	delete m_camera;
	//	m_camera = NULL;
	//}
	//if (m_light != NULL)
	//{
	//	m_light->Destroy();
	//	delete m_light;
	//	m_light = NULL;
	//}
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

//void DScene::AddTransform(DTransform * transform)
//{
//	if (m_transforms == NULL)
//		return;
//	transform->RemoveFromParent();
//	m_transforms->push_back(transform);
//}

void DScene::AddSceneObject(DSceneObject * sceneObj)
{
	if (m_rootTransform == NULL)
		return;
	//if (m_transforms == NULL)
	//	return;
	// sceneObj->GetTransform()->RemoveFromParent();
	sceneObj->GetTransform()->SetParent(m_rootTransform);
	//m_transforms->push_back(sceneObj->GetTransform());
}

DCameraNode * DScene::GetCameraNode()
{
	return m_cameraNode;
}

void DScene::SetCameraNode(DCameraNode * cameraNode)
{
	m_cameraNode = cameraNode;
}

DLightNode * DScene::GetLightNode()
{
	return m_lightNode;
}

void DScene::SetLightNode(DLightNode * lightNode)
{
	m_lightNode = lightNode;
}

void DScene::Draw(bool callOnRender, DCuller * culler, DRender * render)
{
	DScene* current = DSystem::GetSceneMgr()->GetCurrentScene();
	if (current != NULL)
	{
		if (current->m_rootTransform != NULL)
			current->DrawSceneObject(current->m_rootTransform, culler, render);

		if (callOnRender)
			current->OnRender();
	}
}

void DScene::DrawShadow()
{
	DScene* current = DSystem::GetSceneMgr()->GetCurrentScene();
	//if (current != NULL && current->m_light != NULL)
	//	current->m_light->RenderShadow();
	DLightNode* node = current->m_lightNode;
	while (node != NULL)
	{
		node->light->RenderShadow();
		node = node->next;
	}
}

void DScene::UpdateSceneObject(DTransform * node)
{
	DTransform* child = node->GetFirstChild();
	while (child != NULL)
	{
		DSceneObject* sobj = child->GetSceneObject();
		if (sobj != NULL) {
			sobj->Update();
		}
		UpdateSceneObject(child);

		child = child->GetNextNegibhor();
	}
}

void DScene::FixedUpdateSceneObject(DTransform * node)
{
	DTransform* child = node->GetFirstChild();
	while (child != NULL)
	{
		DSceneObject* sobj = child->GetSceneObject();
		if (sobj != NULL)
			sobj->FixedUpdate();
		FixedUpdateSceneObject(child);

		child = child->GetNextNegibhor();
	}
}

void DScene::UnLoadSceneObject(DTransform * node)
{
	DTransform* child = node->GetFirstChild();
	while (child != NULL)
	{
		UnLoadSceneObject(child);

		DSceneObject* sobj = child->GetSceneObject();

		child = child->GetNextNegibhor();

		if (sobj != NULL) {
			sobj->Destroy();
			delete sobj;
			sobj = NULL;
		}
	}
}

void DScene::DrawSceneObject(DTransform * node, DCuller * culler, DRender * render)
{
	DTransform* child = node->GetFirstChild();
	while (child != NULL)
	{
		DSceneObject* sobj = child->GetSceneObject();
		if (sobj != NULL) {
			if (sobj->CullObject(culler))
			{
				//render->PushDisplayObject(sobj, sobj->)
			}
		}
		DrawSceneObject(child, culler, render);

		child = child->GetNextNegibhor();
	}
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

//void DScene::AddDisplayObject(DDisplayObject * displayObject)
//{
//	if (m_displayObjects == NULL)
//		return;
//	m_displayObjects->push_back(displayObject);
//}
//
//void DScene::SetCamera(DCamera * camera)
//{
//	m_camera = camera;
//}
//
//DCamera * DScene::GetCamera()
//{
//	return m_camera;
//}
//
//void DScene::SetLight(DLight * light)
//{
//	m_light = light;
//}
//
//DLight * DScene::GetLight()
//{
//	return m_light;
//}