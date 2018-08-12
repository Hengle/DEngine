#include "stdafx.h"
#include "WaterScene.h"
#include "DSystem.h"
#include "DGUI.h"
#include "TestResDefine.h"


WaterScene::WaterScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
	m_camera = 0;
	m_lookDistance = 7.0f;
}

WaterScene::~WaterScene()
{
}

void WaterScene::OnGUI()
{
}

void WaterScene::OnLoad()
{
	m_camera = new DCamera();
	m_camera->Create();
	
	m_filter = new WaterFilter();
	m_camera->SetFilter(m_filter);

	DTransform* transform = m_camera->GetTransform();

	transform->SetPosition(-2.418302f, 9.734123f, -13.54027);
	transform->SetEuler(35.065f, 19.253f, 0.0f);

	DGeometry* geo = DRes::Load<DGeometry>(DEFAULT_GROUP, 4002);
	DMaterial* mat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3014);
	
	DDisplayObject* poolobj = new DDisplayObject(geo, mat);
	poolobj->Create();

	geo = DRes::Load<DGeometry>(DEFAULT_GROUP, 4004);
	mat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3015);

	DDisplayObject* buildingObj = new DDisplayObject(geo, mat);
	buildingObj->Create();
	transform = buildingObj->GetTransform();
	transform->SetPosition(-1.4f, 0.72f, -4.08f);
	transform->SetEuler(0, -34.144f, 0);

	geo = DRes::Load<DGeometry>(DEFAULT_GROUP, 4003);
	mat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3016);

	DDisplayObject* waterObj = new DDisplayObject(geo, mat);
	waterObj->Create();


	DLight* light = new DLight();
	light->Create();
	transform = light->GetTransform();
	transform->SetEuler(50.0f, -30.0f, 0.0f);
}

void WaterScene::OnUnLoad()
{
	if (m_filter != NULL)
	{
		m_filter->Release();
		delete m_filter;
		m_filter = 0;
	}

	DRes::UnLoadGroup(DEFAULT_GROUP);
}

void WaterScene::OnUpdate()
{
	if (DInput::IsMousePress(0) && !DGUI::IsGUIActive())
	{
		int dtx, dty;
		DInput::GetDeltaMouseMove(dtx, dty);
		DVector3 euler;
		DVector3 forward;
		m_camera->GetTransform()->GetEuler(euler);

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
