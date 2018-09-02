#pragma once
#include "DScene.h"
#include "MyBone.h"

class BoneScene : public DScene
{
public:
	BoneScene(SCENEID sceneId, char* sceneName);

protected:
	virtual void OnGUI();
	virtual void OnLoad();
	virtual void OnUnLoad();
	virtual void OnUpdate();

private:
	void DrawBone(MyBone*, int index);

private:
	DCamera* m_camera;
	MyBoneObj* m_bone;
	DVector3 m_lookAtPoint;
	float m_lookDistance;
};

