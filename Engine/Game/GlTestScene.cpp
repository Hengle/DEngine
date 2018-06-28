#include "stdafx.h"
#include "GlTestScene.h"

GlTestScene::GlTestScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
}

void GlTestScene::OnGUI()
{
}

void GlTestScene::OnLoad()
{
	m_testcolorshader = DShader::Create(L"../Res/color.vs", L"../Res/color.ps");
	m_testcolormat = new DMaterial(m_testcolorshader);

	DCamera * cam = new DCamera();

	DTransform* transform = cam->GetTransform();
	transform->SetPosition(-3.0437f, 9.261985f, -10.086f);
	transform->SetEuler(31.319f, 16.501f, 0);
	cam->SetBackgroundColor(DColor(49.0f / 255.0f, 77.0f / 255.0f, 121.0f / 255.0f));
}

void GlTestScene::OnUnLoad()
{
	if (m_testcolormat != NULL)
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
	}
}

void GlTestScene::OnRender()
{
}

void GlTestScene::OnUpdate()
{
}
