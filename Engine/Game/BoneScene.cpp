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

void BoneScene::OnGUI()
{
	int boneCount = m_bone->GetBoneCount();
	int i;

	DTransform* transform;
	DVector3 euler;

	transform = m_bone->GetTransform();
	transform->GetEuler(euler);

	ImGui::SliderFloat("bonePitch", &euler.x, -180.0f, 180.0f);
	ImGui::SliderFloat("boneYaw", &euler.y, -180.0f, 180.0f);
	ImGui::SliderFloat("boneRoll", &euler.z, -180.0f, 180.0f);
	
	transform->SetEuler(euler);

	for (i = 0; i < boneCount; i++)
	{
		MyBone* bone = m_bone->GetBone(i);
		DrawBone(bone, i);

		//transform->SetLocalEuler(x, y, z);
	}
}

void BoneScene::OnLoad()
{
	m_camera = new DCamera();
	m_camera->Create();

	DTransform* transform = m_camera->GetTransform();
	transform->SetPosition(-0.4731708f, 1.009849f, 0.70194f);
	transform->SetEuler(34.378f, 143.56f, 0);
	m_camera->SetBackgroundColor(DColor(49.0f / 255.0f, 77.0f / 255.0f, 121.0f / 255.0f));

	m_bone = new MyBoneObj();
	m_bone->Create();

	transform = m_bone->GetTransform();
	transform->SetPosition(0.0f, 0.0f, 0.0f);
	transform->SetEuler(-90.0f, 0.0f, 0.0f);

	m_bone->LoadBone("../Res/bone.txt");
	m_bone->LoadAnim("../Res/anim.txt");

	m_bone->Play();

	DMaterial* mat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3017);
	DGeometry* geo = DRes::Load<DGeometry>(DEFAULT_GROUP, 4005);

	m_geo = new DDisplayObject(geo, mat);
	m_geo->Create();
	transform = m_geo->GetTransform();
	transform->SetPosition(0.0f, 0.0f, 0.0f);
	//transform->SetEuler(-90.0f, 0.0f, 0.0f);

	int i, boneCount;
	boneCount = m_bone->GetBoneCount();
	m_geo->GetGeometry()->SetBoneMatrixCount(boneCount);

	//DMatrix4x4 ltw;
	//transform->GetLocalToWorld(ltw);

	for (i = 0; i < boneCount; i++)
	{
		DMatrix4x4 matrix;
		m_bone->GetBoneMatrix(i, matrix);
		m_geo->GetGeometry()->SetBoneMatrix(matrix, i);
	}

	transform->GetWorldToLocal(wTL);

	//DGeometry* cube = DRes::LoadInternal<DGeometry>(D_RES_MESH_CUBE);
	//DMaterial* mat3 = DRes::Load<DMaterial>(DEFAULT_GROUP, DECAL_MAT);
	////mat3->SetZTest(DRSCompareFunc_Greater);
	//DDisplayObject* cubeobj = new DDisplayObject(cube, mat3);
	//cubeobj->Create();
	//DTransform* tr = cubeobj->GetTransform();
	//tr->SetPosition(0.004f, 0.941f, 0.057f);
	//tr->SetParent(transform);
}

void BoneScene::OnUnLoad()
{
	DRes::UnLoadGroup(DEFAULT_GROUP);
}

void BoneScene::OnUpdate()
{
	
	m_geo->GetGeometry()->UpdateBone(m_bone->GetBones(), wTL);



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

		m_lookDistance += dty*0.05f;
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

		m_lookAtPoint = camUp*dty*0.03f + m_lookAtPoint;
		m_lookAtPoint = camRight*dtx*-0.03f + m_lookAtPoint;

		DVector3 forward;
		m_camera->GetTransform()->GetForward(forward);
		DVector3 position = m_lookAtPoint - forward*m_lookDistance;
		m_camera->GetTransform()->SetPosition(position);
	}
}

void BoneScene::DrawBone(MyBone * bone, int index)
{
	DTransform* transform;
	transform = bone->GetTransform();

	/*DVector3 euler;

	const char* boneName = bone->GetBoneName();
	ImGui::Text("%s,index:%d",boneName, index);

	DTransform* parent = transform->GetParent();
	if (parent != NULL)
	{
		MyBone* parentObj = (MyBone*)(parent->GetSceneObject());
		ImGui::Text("Parent:%s", parentObj->GetBoneName());
	}

	transform->GetLocalEuler(euler);

	char pitch[64], yall[64], roll[64];

	sprintf_s(pitch, "   Pitch:%s", boneName);
	sprintf_s(yall, "   Yall:%s", boneName);
	sprintf_s(roll, "   Roll:%s", boneName);

	ImGui::SliderFloat(pitch, &euler.x, -180.0f, 180.0f);
	ImGui::SliderFloat(yall, &euler.y, -180.0f, 180.0f);
	ImGui::SliderFloat(roll, &euler.z, -180.0f, 180.0f);

	transform->SetLocalEuler(euler);*/


	DVector3 position;

	const char* boneName = bone->GetBoneName();
	ImGui::Text("%s,index:%d", boneName, index);

	DTransform* parent = transform->GetParent();
	if (parent != NULL)
	{
		MyBone* parentObj = (MyBone*)(parent->GetSceneObject());
		ImGui::Text("Parent:%s", parentObj->GetBoneName());
	}

	transform->GetLocalPosition(position);

	/*char x[64], y[64], z[64];

	sprintf_s(x, "   X:%s", boneName);
	sprintf_s(y, "   Y:%s", boneName);
	sprintf_s(z, "   Z:%s", boneName);*/

	ImGui::Text("X:%f", position.x);
	ImGui::Text("Y:%f", position.y);
	ImGui::Text("Z:%f", position.z);

	//transform->SetLocalEuler(euler);
}
