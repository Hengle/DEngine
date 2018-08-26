#include "stdafx.h"
#include "BoneScene.h"
#include "DRes.h"
#include "TestResDefine.h"

BoneScene::BoneScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
}

void BoneScene::OnLoad()
{
	m_camera = new DCamera();
	m_camera->Create();

	DTransform* transform = m_camera->GetTransform();
	transform->SetPosition(-0.3505601f, 1.020166f, 0.7136378f);
	transform->SetEuler(36.44f, 152.498f, 0);
	m_camera->SetBackgroundColor(DColor(49.0f / 255.0f, 77.0f / 255.0f, 121.0f / 255.0f));

	m_bone = new MyBoneObj();
	m_bone->Create();

	m_bone->LoadBone("../Res/bone.txt");
}

void BoneScene::OnUnLoad()
{
	DRes::UnLoadGroup(DEFAULT_GROUP);
}

void BoneScene::OnUpdate()
{
}
