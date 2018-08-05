#include "stdafx.h"
#include "GlTestScene.h"
#include "DGraphics.h"
#include "DRes.h"
#include "TestResDefine.h"

GlTestScene::GlTestScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
}

void GlTestScene::OnGUI()
{
}

void GlTestScene::OnLoad()
{
	//m_testcolorshader = DRes::Load<DShader>(DEFAULT_GROUP, COLOR_SHADER);
	//m_testcolormat = new DMaterial(m_testcolorshader);
	m_testcolormat = DRes::Load<DMaterial>(DEFAULT_GROUP, COLOR_MAT);

	cam = new DCamera();
	cam->Create();

	DTransform* transform = cam->GetTransform();
	transform->SetPosition(-11.47823f, 13.5114f, -19.58396f);
	transform->SetEuler(35.444f, 29.564f, 0);
	cam->SetBackgroundColor(DColor(49.0f / 255.0f, 77.0f / 255.0f, 121.0f / 255.0f));

	//SetCamera(cam);
}

void GlTestScene::OnUnLoad()
{
	/*if (m_testcolormat != NULL)
	{
		m_testcolormat->Destroy();
		delete m_testcolormat;
		m_testcolormat = NULL;
	}
	if (m_testcolorshader != NULL)
	{
		m_testcolorshader->Destroy();
		delete m_testcolorshader;
		m_testcolorshader = NULL;
	}*/
	DRes::UnLoadGroup(DEFAULT_GROUP);
}

void GlTestScene::OnRender()
{
	//DGraphics::GlSetMaterial(m_testcolormat);
	m_testcolormat->SetPass(0);
	DGraphics::GlPushMatrix();

	DGraphics::GlBegin(DGeometryTopology_LineList);

	DGraphics::GlVertex3(0.0f, 0.0f, 0.0f);
	DGraphics::GlVertex3(-1.0f, 1.0f, -1.0f);

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();


	if (cam != NULL)
	{
		DTransform* tran = cam->GetTransform();
		DMatrix4x4 ltw;
		tran->GetLocalToWorld(ltw);
		DGraphics::GlPushMatrix();
		DGraphics::GlMultiMatrix(ltw);
		DGraphics::GlBegin(DGeometryTopology_LineList);

		DGraphics::GlVertex3(0.0f, 0.0f, 10.0f);
		DGraphics::GlVertex3(-2.0f, 3.0f, 20.0f);

		DGraphics::GlEnd();

		DGraphics::GlPopMatrix();
	}

	DGraphics::GlPushMatrix();
	DGraphics::GlLoadOrtho();
	DGraphics::GlBegin(DGeometryTopology_LineList);

	DGraphics::GlVertex3(-1.0f, -1.0f, 0.0f);
	DGraphics::GlVertex3(1.0f, 1.0f, 0.0f);

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();
}

void GlTestScene::OnUpdate()
{
}
