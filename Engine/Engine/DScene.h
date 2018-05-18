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
	void Render(ID3D11DeviceContext*);
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

protected:
	std::vector<DDisplayObject*>* m_displayObjects;
	DCamera* m_camera;

private:
	bool m_isLoaded;
	bool m_isEnter;
	char* m_sceneName;
	SCENEID m_sceneId;
};

