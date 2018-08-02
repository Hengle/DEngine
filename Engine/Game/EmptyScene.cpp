#include "stdafx.h"
#include "EmptyScene.h"
#include "DRes.h"
#include "TestResDefine.h"
#include "DSystem.h"
#include "DGUI.h"

EmptyScene::EmptyScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
	m_camera = 0;
	m_lookDistance = 7.0f;
}

void EmptyScene::OnLoad()
{
	m_camera = new DCamera();
	m_camera->Create();

	DTransform* transform;

	transform = m_camera->GetTransform();
	transform->SetEuler(33.346f, -41.563f, 0.0f);
	transform->SetPosition(3.975387f, 2.588932f, -3.703708f);

	DMaterial* colmat = DRes::Load<DMaterial>(DEFAULT_GROUP, PLANEBUMP_MAT);
	DGeometry* obj = DRes::Load<DGeometry>(DEFAULT_GROUP, PLANE_MESH);
	//DGeometry* obj = DGeometry::Create(DGeometry_Sphere);
	//DGeometry* obj = DGeometry::Create(DGeometry_Plane);

	DDisplayObject* obj0 = new DDisplayObject(obj, colmat);
	obj0->Create();


	DLight* light = new DLight();
	light->Create();
	transform = light->GetTransform();
	transform->SetEuler(50.0f, -30.0f, 0.0f);

}

void EmptyScene::OnUnLoad()
{
	DRes::UnLoadGroup(DEFAULT_GROUP);
}

void EmptyScene::OnUpdate()
{

	//if (DInput::IsKeyDown(0x10))
	if (DInput::IsKeyDown(81))
	{
		DTexture2D* tex = DRes::Load<DTexture2D>(0, 2003);
		tex->SetWrapMode(DWrapMode_Repeat);
	}
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
