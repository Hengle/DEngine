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

	DCamera * cam = new DCamera();

	DTransform* transform = cam->GetTransform();
	transform->SetPosition(-11.47823f, 13.5114f, -19.58396f);
	transform->SetEuler(35.444f, 29.564f, 0);
	cam->SetBackgroundColor(DColor(49.0f / 255.0f, 77.0f / 255.0f, 121.0f / 255.0f));

	SetCamera(cam);
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
}

void GlTestScene::OnRender()
{
	DGraphics::GlSetMaterial(m_testcolormat);
	DGraphics::GlPushMatrix();

	DGraphics::GlBegin();

	DGraphics::GlVector(0.0f, 0.0f, 0.0f);
	DGraphics::GlVector(-1.0f, 1.0f, -1.0f);

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();


	DCamera* cam = GetCamera();
	if (cam != NULL)
	{
		DTransform* tran = cam->GetTransform();
		DMatrix4x4 ltw;
		tran->GetLocalToWorld(ltw);
		DGraphics::GlPushMatrix();
		DGraphics::GlMultiMatrix(ltw);
		DGraphics::GlBegin();

		DGraphics::GlVector(0.0f, 0.0f, 10.0f);
		DGraphics::GlVector(-2.0f, 3.0f, 20.0f);

		DGraphics::GlEnd();

		DGraphics::GlPopMatrix();
	}

	DGraphics::GlPushMatrix();
	DGraphics::GlLoadOrtho();
	DGraphics::GlBegin();

	DGraphics::GlVector(-1.0f, -1.0f, 0.0f);
	DGraphics::GlVector(1.0f, 1.0f, 0.0f);

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();
}

void GlTestScene::OnUpdate()
{
}
