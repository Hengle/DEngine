#pragma once
#include "DCamera.h"
#include "DDisplayObject.h"
#include <vector>

typedef unsigned int        SCENEID;

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

	static void Draw(bool callOnRender, DShader* replaceShader = NULL);
	static void DrawShadow();

protected:
	virtual void OnGUI();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnLoad();
	virtual void OnUnLoad();

	void AddDisplayObject(DDisplayObject*);
	void SetCamera(DCamera*);
	DCamera* GetCamera();
	void SetLight(DLight*);
	DLight* GetLight();

private:
	void DrawScene(bool callOnRender, DShader* replaceShader);

protected:
	std::vector<DDisplayObject*>* m_displayObjects;
	DCamera* m_camera;
	DLight* m_light;

private:
	bool m_isLoaded;
	bool m_isEnter;
	char* m_sceneName;
	SCENEID m_sceneId;
};
