#include "stdafx.h"
#include "BoneScene.h"
#include "DRes.h"
#include "TestResDefine.h"
#include "DSystem.h"
#include "DGUI.h"

BoneScene::BoneScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
	m_camera = 0;
	m_lookDistance = 7.0f;
}

void BoneScene::OnLoad()
{
	m_camera = new DCamera();
	m_camera->Create();

	DTransform* transform = m_camera->GetTransform();
	transform->SetPosition(-0.6467111f, 1.890899f, 1.754533f);
	transform->SetEuler(24.511f, 156.074f, 0);
	m_camera->SetBackgroundColor(DColor(49.0f / 255.0f, 77.0f / 255.0f, 121.0f / 255.0f));

	m_bone = new MyBoneObj();
	m_bone->Create();

	transform = m_bone->GetTransform();
	transform->SetPosition(0.0f, 1.054817f, 0.01587593f);
	transform->SetEuler(-90.0f, 89.9999f, 0.0f);

	m_bone->LoadBone("../Res/bone.txt");
	m_bone->LoadAnim("../Res/anim.txt");
}

void BoneScene::OnUnLoad()
{
	DRes::UnLoadGroup(DEFAULT_GROUP);
}

void BoneScene::OnUpdate()
{
	if (DInput::IsMousePress(0) && !DGUI::IsGUIActive())
	{
		int dtx, dty;
		DInput::GetDeltaMouseMove(dtx, dty);
		DVector3 euler;
		DVector3 forward;
		m_camera->GetTransform()->GetEuler(euler);

		//euler.y += DTime::GetDeltaTime()*20.0f;
		euler.y += dtx;
		euler.x += dty;

		m_camera->GetTransform()->SetEuler(euler);

		m_camera->GetTransform()->GetForward(forward);

		DVector3 position = m_lookAtPoint - forward*m_lookDistance;
		m_camera->GetTransform()->SetPosition(position);
	}
	if (DInput::IsMousePress(1) && !DGUI::IsGUIActive())
	{
		int dtx, dty;
		DInput::GetDeltaMouseMove(dtx, dty);

		m_lookDistance += dty*0.2f;
		DVector3 forward;
		m_camera->GetTransform()->GetForward(forward);
		DVector3 position = m_lookAtPoint - forward*m_lookDistance;
		m_camera->GetTransform()->SetPosition(position);
	}
	if (DInput::IsMousePress(2) && !DGUI::IsGUIActive())
	{
		int dtx, dty;
		DInput::GetDeltaMouseMove(dtx, dty);
		DTransform* transform;
		transform = m_camera->GetTransform();

		DVector3 camRight, camUp;
		transform->GetRight(camRight);
		transform->GetUp(camUp);

		m_lookAtPoint = camUp*dty*0.1f + m_lookAtPoint;
		m_lookAtPoint = camRight*dtx*-0.1f + m_lookAtPoint;

		DVector3 forward;
		m_camera->GetTransform()->GetForward(forward);
		DVector3 position = m_lookAtPoint - forward*m_lookDistance;
		m_camera->GetTransform()->SetPosition(position);
	}
}
