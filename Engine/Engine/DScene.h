#pragma once
#include "DCamera.h"
#include "DDisplayObject.h"

typedef unsigned int        SCENEID;

/*场景*/
class DScene : public DObject
{
public:
	DScene(SCENEID, char*);
	~DScene();
	void DrawGUI();
	void Render();
	void Update();
	void FixedUpdate();
	void Enter();
	void Exit();
	void Load();
	void UnLoad();
	char* GetSceneName();
	SCENEID GetSceneId();
	bool IsSceneLoaded();
	bool IsEnter();
	virtual void Destroy();

	void AddSceneObject(DSceneObject*);
	DCameraNode* GetCameraNode();
	void SetCameraNode(DCameraNode*);
	DLightNode* GetLightNode();
	void SetLightNode(DLightNode*);

	static void Draw(bool callOnRender);
	static void DrawShadow();

private:
	void UpdateSceneObject(DTransform* node);
	void FixedUpdateSceneObject(DTransform* node);
	void UnLoadSceneObject(DTransform* node);
	void DrawSceneObject(DTransform* node);

protected:
	virtual void OnGUI();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnLoad();
	virtual void OnUnLoad();

	//void SetCamera(DCamera*);
	//DCamera* GetCamera();
	//void SetLight(DLight*);
	//DLight* GetLight();

protected:
	//std::vector<DTransform*>* m_transforms;
	DTransform* m_rootTransform;
	DCameraNode* m_cameraNode;
	DLightNode* m_lightNode;
	//DCamera* m_camera;
	//DLight* m_light;

private:
	bool m_isLoaded;
	bool m_isEnter;
	char* m_sceneName;
	SCENEID m_sceneId;
};
