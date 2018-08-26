#pragma once
#include "DScene.h"
#include "MyBone.h"

class BoneScene : public DScene
{
public:
	BoneScene(SCENEID sceneId, char* sceneName);

protected:
	virtual void OnLoad();
	virtual void OnUnLoad();
	virtual void OnUpdate();

private:
	DCamera* m_camera;
	MyBoneObj* m_bone;
};

