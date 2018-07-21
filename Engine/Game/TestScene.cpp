#include "stdafx.h"
#include "TestScene.h"
//#include "DImGUI.h"
#include "DSystem.h"
#include "DTransform.h"
#include "DLog.h"
#include "DGUI.h"
#include "D3D9Core.h"
#include "TestResDefine.h"
#include "SVCamera.h"

TestScene::TestScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
	//m_Model = 0;
	//m_ColorShader = 0;
	m_plane = 0;
	m_cube = 0;
	m_camera = 0;
	m_obj0 = 0;
	plane = 0;
	/*shader = 0;
	mat = 0;
	mat2 = 0;
	mat3 = 0;*/
	//cube = 0;
	//obj = 0;

	m_light = 0;
	m_lookDistance = 17.0f;
}

void TestScene::OnGUI()
{
	DGUI::Label(u8"森哥的引擎：中文utf-8测试");
	DGUI::Label("DeltaTime:%lf", DTime::GetDeltaTime());
	DGUI::Label("FixedDeltaTime:%lf", DTime::GetFixedDeltaTime());
	DGUI::Label("Timer:%lf", DTime::GetTimer());
	DGUI::Label("FPS:%d", DTime::GetFPS());
	DGUI::Label("DrawCall:%d", DGraphics::GetDrawCall());

	if (DGUI::Button("Test Destroy"))
	{
		m_obj0->Destroy();
		delete m_obj0;
		m_obj0 = 0;
	}
	//DGUI::Label("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	/*D3DXVECTOR3 euler;
	DTransform* transform;
	transform = m_obj->GetTransform();
	transform->GetEuler(euler);

	ImGui::SliderFloat("Pitch", &euler.x, 0.0f, 360.0f);
	ImGui::SliderFloat("Yall", &euler.y, 0.0f, 360.0f);
	ImGui::SliderFloat("Roll", &euler.z, 0.0f, 360.0f);*/

	/*if (DInput::IsMousePress(0)) 
	{
		DGUI::Label("Mouse Down");
	}
	if (DInput::IsMouseDown(0))
	{
		DLog::Info("Mouse Click");
	}
	if (DInput::IsKeyPress(DIK_A))
	{
		DGUI::Label("Key A Down");
	}
	if (DInput::IsKeyDown(DIK_A))
	{
		DLog::Info("Key A Click");
	}*/
	
	/*ImGui::Begin("xa", 0, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoInputs);
	if (ImGui::Button("Pitch"))
	{
		DLog::Info("xas");
	}
	ImGui::Text("xas");
	ImGui::End();*/



	//transform->SetEuler(euler.x, euler.y, euler.z);

	DTransform* transform = m_light->GetTransform();
	DVector3 euler, forward;
	transform->GetEuler(euler);

	ImGui::SliderFloat("LPitch", &euler.x, 0.0f, 360.0f);
	ImGui::SliderFloat("LYall", &euler.y, 0.0f, 360.0f);
	ImGui::SliderFloat("LRoll", &euler.z, 0.0f, 360.0f);


	transform->SetEuler(euler.x, euler.y, euler.z);

	if (m_obj0 != NULL)
	{
		transform = m_obj0->GetTransform();

		/*DVector3 right, up, forward, position;
		transform->GetRight(right);
		transform->GetUp(up);
		transform->GetForward(forward);
		transform->GetPosition(position);

		DGUI::Label("Right:(%f,%f,%f)", right.x, right.y, right.z);
		DGUI::Label("Up:(%f,%f,%f)", up.x, up.y, up.z);
		DGUI::Label("Forward:(%f,%f,%f)", forward.x, forward.y, forward.z);*/

		DVector3 obje, objp, objs;
		transform->GetEuler(obje);
		transform->GetPosition(objp);
		transform->GetLocalScale(objs);

		ImGui::SliderFloat("objPos", &objp.x, 0.0f, 3.0f);
		ImGui::SliderFloat("objRot", &obje.y, 0.0f, 360.0f);
		ImGui::SliderFloat("objSize", &objs.x, 0.1f, 3.0f);

		transform->SetPosition(objp);
		transform->SetEuler(obje);
		transform->SetLocalScale(objs);
	}
	if (m_cube != NULL)
	{
		transform = m_cube->GetTransform();

		/*DVector3 right, up, forward;
		transform->GetRight(right);
		transform->GetUp(up);
		transform->GetForward(forward);

		DGUI::Label("Right:(%f,%f,%f)", right.x, right.y, right.z);
		DGUI::Label("Up:(%f,%f,%f)", up.x, up.y, up.z);
		DGUI::Label("Forward:(%f,%f,%f)", forward.x, forward.y, forward.z);*/

		
		/*float posLerp = 0, eulerV = 0, sizeV = 1.0;
		ImGui::SliderFloat("cubePos", &posLerp, 0.0f, 1.0f);
		ImGui::SliderFloat("cubeRot", &eulerV, 0.0f, 360.0f);
		ImGui::SliderFloat("cubeSize", &sizeV, 0.1f, 3.0f);

		DVector3 pos;
		DVector3::Lerp(DVector3(2.64f, 2.61f, 0.0f), DVector3(1.2f, 2.0f, 0.4f), posLerp, pos);
		transform->SetPosition(pos);
		transform->SetEuler(0.0f, eulerV, 0.0f);
		transform->SetLocalScale(sizeV, sizeV, sizeV);*/
	}
	//transform->GetForward(forward);
	//DShader::SetGlobalVector3("g_sundir", forward);
}

void TestScene::OnLoad()
{
	/*DShaderBlock* block = new DShaderBlock();
	block->Compile("../Res/color.shader");

	if (block != NULL)
	{
		block->Release();
		delete block;
		block = NULL;
	}*/
	//DShader* testshader = DShader::Create(L"../Res/color.vs9", L"../Res/color.ps9");
	//if (testshader != NULL)
	//{
	//	testshader->Destroy();
	//	delete testshader;
	//	testshader = NULL;
	//}

	//testd = new MyTestDraw();
	//testd->Init();


	//core->GetDevice()->CreateVertexBuffer(3 * sizeof(float)*7, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &m_mesh, 0);

	TestLoad();

	////DLog::Err(u8"打印个日志测试");
	////DLog::Warn("Log Test");

	////m_Model = new ModelClass;
	////

	////m_Model->Initialize(DSystem::GetGraphicsCore()->GetDevice());

	////m_ColorShader = new ColorShaderClass;

	////m_ColorShader->Initialize(DSystem::GetGraphicsCore()->GetDevice(), DSystem::GetHWND());

	////DTransform* trs = new DTransform();
	//////trs->SetEuler(-56.442f, 22.605f, 7.83f);
	////trs->SetPosition(2.51f, 1.48f, -3.05f);
	////trs->SetRotation(-0.3052f, 0.3219f, 0.4695f, 0.7634f);
	////trs->SetScale(0.5f, 1.5f, 1.f);

	//////D3DXQUATERNION rot;
	//////trs->GetRotation(rot);
	////D3DXMATRIX ltw;
	////D3DXVECTOR3 up, forward, right;
	////trs->GetLocalToWorld(ltw);
	////trs->GetUp(up);
	////trs->GetForward(forward);
	////trs->GetRight(right);

	////delete trs;
	////trs = NULL;

	////DLog::Info("casdfaq");
	////DLog::Err("xasw");
	////DLog::Warn("xaasdfaqwerasdfzxcvasdfqwerasdfzxcvxzcvasdfqwerasdfasdfzxcvzxcasdfwqw2");
	////DLog::Info("4");
	////DLog::Info("5");
	////DLog::Info("6");
	////DLog::Info("7");
}

void TestScene::TestLoad()
{
	SVCamera* svc = new SVCamera();
	svc->Create();

	DMaterial* skymat = DRes::Load<DMaterial>(DEFAULT_GROUP, SKY_MAT);

	m_testColorMat = DRes::Load<DMaterial>(DEFAULT_GROUP, COLOR_MAT);
	/*testcolormesh = new DMesh();
	float* vs = new float[12];
	unsigned long* is = new unsigned long[4];

	vs[0] = 0.0f; vs[1] = 0.0f; vs[2] = 0.0f;
	vs[3] = 3.0f; vs[4] = 3.0f; vs[5] = 3.0f;
	vs[6] = 3.0f; vs[7] = 3.0f; vs[8] = 3.0f;
	vs[9] = 0.0f; vs[10] = 6.0f; vs[11] = 4.1f;

	is[0] = 0; is[1] = 1;
	is[2] = 2; is[3] = 3;

	testcolormesh->SetVertices(vs, 4);
	testcolormesh->SetIndices(is, 4);
	testcolormesh->SetTopology(DMeshTopology_LineList);

	testcolorobj = new DDisplayObject(testcolormesh, testcolormat);
	AddDisplayObject(testcolorobj);*/


	/*DShader* aoshader = DShader::Create(L"../Res/testv9.v9", L"../Res/texture.ps9");
	aoshader->Destroy();
	delete aoshader;
	aoshader = NULL;*/

	

	m_camera = new DCamera();
	m_camera->Create();
	DTransform* transform;

	//DShader* replacetest = DRes::Load<DShader>(DEFAULT_GROUP, REPLACE_SHADER);
	//cam->SetReplaceShader(replacetest);

	float w, h;
	DSystem::GetGraphicsMgr()->GetResolution(w, h);
	m_rt = DRenderTexture::Create(w,h);
	//cam->SetRenderTexture(m_rt);

	m_filter = new TestFilter();
	//cam->SetFilter(m_filter);
	m_camera->SetSkyBox(skymat);

	transform = m_camera->GetTransform();
	transform->SetEuler(34.996f, -154.423f, 0.0f);
	transform->SetPosition(3.24f, 6.822f, 7.701f);
	//SetCamera(cam);

	m_light = new DLight();
	m_light->Create();
	m_light->SetColor(1, 1, 1, 1);
	m_light->SetFar(60.0f);
	transform = m_light->GetTransform();
	transform->SetEuler(50, -30, 0);
	transform->SetPosition(5.05f, 12.04f, -8.74f);

	//DMesh* mesh = DMesh::Create("../Res/eboy.obj");
	plane = DGeometry::Create(DGeometry_Plane);
	//shader = DRes::Load<DShader>(DEFAULT_GROUP, TEXTURE_SHADER);
	//floor = DRes::Load<DTexture2D>(DEFAULT_GROUP, DECAL_TEX);
	//map = DRes::Load<DTexture2D>(DEFAULT_GROUP, BODY_TEX);
	//cb = DRes::Load<DTexture2D>(DEFAULT_GROUP, GROUND_TEX);
	DMaterial* mat = DRes::Load<DMaterial>(DEFAULT_GROUP, DECAL_MAT);
	//mat = new DMaterial(shader);

	//mat->SetFloat("power", 1.3f);
	//mat->SetColor("vcolor", DColor(1.0f, 0.0f, 0.0f, 1.0f));
	//mat->SetTexture("shaderTexture", floor);

	////DTexture2D* texture = new DTexture2D(L"../Res/eboy.tif");
	////DTexture2D* decal = new DTexture2D(L"../Res/decal.jpg");
	m_plane = new DDisplayObject(plane, mat);
	transform = m_plane->GetTransform();
	m_plane->Create();


	//AddDisplayObject(m_plane);

	//obj = DMesh::Create("../Res/eboy.obj");
	DGeometry* obj = DRes::Load<DGeometry>(DEFAULT_GROUP, BODY_MESH);
	// mat2 = new DMaterial(shader);
	DMaterial* mat2 = DRes::Load<DMaterial>(DEFAULT_GROUP, BODY_MAT);

	DMaterial* mat4 = DRes::Load<DMaterial>(DEFAULT_GROUP, TRANSPARENT_MAT);
	//mat2->SetCullMode(DCullMode_Front);

	//mat2->SetFloat("power", 1.6f);
	//mat2->SetColor("vcolor", DColor(0.0f, 1.0f, 0.0f, 1.0f));
	//mat2->SetTexture("shaderTexture", map);

	m_obj0 = new DDisplayObject(obj, mat2);
	m_obj0->Create();
	transform = m_obj0->GetTransform();
	transform->SetPosition(0.0f, 1.64f, 0.0f);
	transform->SetEuler(0.0f, 65.979f, 0.0f);

	m_tobj0 = new DDisplayObject(obj, mat4);
	m_tobj0->Create();
	transform = m_tobj0->GetTransform();
	transform->SetPosition(2.0f, 1.64f, 3.6f);

	//AddDisplayObject(m_obj0);


	//cube = DMesh::Create("../Res/eboy.obj");
	DGeometry* cube = DRes::Load<DGeometry>(DEFAULT_GROUP, BODY_MESH);
	//mat3 = new DMaterial(shader);
	//mat3->SetTexture("shaderTexture", cb);
	DMaterial* mat3 = DRes::Load<DMaterial>(DEFAULT_GROUP, OUTLINE_MAT);
	mat3->SetFloat("outline", 0.1f);
	mat3->SetColor("outlinecolor", DColor(0,0,0,1.0f));
	//mat3->SetZTest(DRSCompareFunc_Greater);
	m_cube = new DDisplayObject(cube, mat3);
	m_cube->Create();
	transform = m_cube->GetTransform();
	transform->SetPosition(2.64f, 2.61f, 0.0f);

	DTransform* objtr = m_obj0->GetTransform();
	transform->SetParent(objtr);
	//AddDisplayObject(m_cube);
}

void TestScene::OnUnLoad()
{
	plane->Destroy();
	delete plane;
	plane = 0;
	/*cube->Destroy();
	delete cube;
	cube = 0;
	obj->Destroy();
	delete obj;
	obj = 0;*/
	/*shader->Destroy();
	delete shader;
	shader = 0;
	mat->Destroy();
	delete mat;
	mat = 0;
	mat2->Destroy();
	delete mat2;
	mat2 = 0;
	mat3->Destroy();
	delete mat3;
	mat3 = 0;
	map->Destroy();
	delete map;
	map = 0;
	floor->Destroy();
	delete floor;
	floor = 0;
	cb->Destroy();
	delete cb;
	cb = 0;*/
	m_filter->Release();
	delete m_filter;
	m_filter = 0;
	m_rt->Destroy();
	delete m_rt;
	m_rt = 0;
	/*m_light->Destroy();
	delete m_light;
	m_light = 0;*/
	//testd->Release();
	//delete testd;
	//testd = NULL;
	/*m_Model->Shutdown();
	delete m_Model;
	m_Model = 0;
	m_ColorShader->Shutdown();
	delete m_ColorShader;
	m_ColorShader = 0;*/
	/*m_obj->Destroy();
	delete m_obj;
	m_obj = NULL;*/
	/*testcolorshader->Destroy();
	delete testcolorshader;
	testcolorshader = 0;
	testcolormat->Destroy();
	delete testcolormat;
	testcolormat = 0;*/
	/*testcolormesh->Destroy();
	delete testcolormesh;
	testcolormesh = 0;*/

	DRes::UnLoadGroup(DEFAULT_GROUP);
}

void TestScene::OnRender()
{
	m_testColorMat->SetPass(0);
	//DGraphics::GlSetMaterial(testcolormat);
	DGraphics::GlPushMatrix();

	DGraphics::GlBegin();

	DGraphics::GlVertex3(0.0f, 0.0f, 0.0f);
	DGraphics::GlVertex3(3.2f, 3.78f, 2.3f);

	/*DGraphics::GlVertex3(3.2f, 3.78f, 2.3f);
	DGraphics::GlVertex3(3.2f, 7.0f, 2.3f);

	DGraphics::GlVertex3(3.2f, 7.0f, 2.3f);
	DGraphics::GlVertex3(-3.2f, 2.0f, -2.3f);*/

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();


	//testd->Render();
	/*DSystem::GetGraphicsCore()->DrawLine(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	DSystem::GetGraphicsCore()->DrawLine(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(-1, 1, 1));
	DSystem::GetGraphicsCore()->DrawLine(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, -1, 1));
	DSystem::GetGraphicsCore()->DrawLine(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, -1));*/
	//D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	//bool result;


	//// Generate the view matrix based on the camera's position.
	//DCamera* cam = GetCamera();

	//// Get the world, view, and projection matrices from the camera and d3d objects.
	//cam->GetViewMatrix(viewMatrix);
	////D3DXMatrixIdentity(&worldMatrix);
	//cam->GetProjection(projectionMatrix);


	//worldMatrix._11 = 0.84501f;
	//worldMatrix._12 = 0.53455f;
	//worldMatrix._13 = 0.01434f;
	//worldMatrix._14 = 0.0f;
	//worldMatrix._21 = -0.17881f;
	//worldMatrix._22 = 0.30772f;
	//worldMatrix._23 = -0.93452f;
	//worldMatrix._24 = 0.0f;
	//worldMatrix._31 = -0.50397f;
	//worldMatrix._32 = 0.78712f;
	//worldMatrix._33 = 0.35561f;
	//worldMatrix._34 = 0.0f;
	//worldMatrix._41 = 3.688f;
	//worldMatrix._42 = 2.51f;
	//worldMatrix._43 = -2.154f;
	//worldMatrix._44 = 1.0f;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model->Render(DSystem::GetGraphicsCore()->GetDeviceContext());

	//// Render the model using the color shader.
	//result = m_ColorShader->Render(DSystem::GetGraphicsCore()->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);



}

void TestScene::OnUpdate()
{
	/*D3DXVECTOR3 euler;
	DTransform* transform;
	transform = m_obj->GetTransform();
	transform->GetEuler(euler);

	euler.y += DTime::GetDeltaTime() * 20;

	transform->SetEuler(euler.x, euler.y, euler.z);*/

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
