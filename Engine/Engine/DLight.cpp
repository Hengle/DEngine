#include "DLight.h"
#include "DShader.h"
#include "DSystem.h"

DLight::DLight()
{
	m_shadowMap = 0;
	m_shadowShader = 0;
}


DLight::~DLight()
{
}

void DLight::GetColor(DColor& color)
{
	color = m_color;
}

void DLight::SetColor(float r, float g, float b, float a)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

void DLight::SetColor(DColor color)
{
	m_color = color;
}

float DLight::GetIntensity()
{
	return m_intensity;
}

void DLight::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

void DLight::RenderShadow()
{
	float w = m_shadowMap->GetWidth();
	float h = m_shadowMap->GetHeight();
	DRect rect = DRect(0, 0, w, h);
	//DGraphics::ResetViewPort();
	DGraphics::SetViewPort(rect);
	BeginRenderShadow();
	DScene::Draw(false, m_shadowShader);
	EndRenderShadow();
}

void DLight::Init()
{
	DSceneObject::Init();
	//float w, h;
	//DSystem::GetGraphicsMgr()->GetResolution(w, h);
	m_shadowMap = DRenderTexture::Create(1024, 1024);
	m_shadowShader = DShader::Create("../Res/shadow.shader");

	DMatrix4x4::Ortho(&m_proj, 30.0f, 30.0f, 0.0f, 34.0f);
	//DMatrix4x4::Perspective(&m_proj, D_PI / 3.0f, 1.0f, 0.03f, 34.0f);
}

void DLight::Destroy()
{
	DSceneObject::Destroy();
	if (m_shadowMap != NULL)
	{
		m_shadowMap->Destroy();
		delete m_shadowMap;
		m_shadowMap = NULL;
	}

	if (m_shadowShader != NULL)
	{
		m_shadowShader->Destroy();
		delete m_shadowShader;
		m_shadowShader = NULL;
	}
}

void DLight::Update()
{
}

void DLight::BeginRenderShadow()
{

	if (m_Transform->IsMatrixWillChange())
	{
		DVector3 up = DVector3(-0.4196f, 0.6816f, -0.5994f);
		DVector3 lookAt = DVector3(-0.3909f, -0.7317f, -0.5584f);
		DVector3 position = DVector3(6.3220f, 13.5694f, 9.0078f);

		lookAt = position + lookAt;

		DMatrix4x4::LookAt(&m_view, position, lookAt, up);
	}

	DVector3 forward;
	m_Transform->GetForward(forward);
	DShader::SetGlobalVector3("g_sundir", forward);

	DGraphics::GlPushMatrix();
	DGraphics::GlLoadIndentity();
	DGraphics::GlMultiMatrix(m_view);
	DGraphics::GlLoadProjectionMatrix(m_proj);

	DShader::SetGlobalVector4("g_shadowParams", DVector4(1.0f, 0.0f, 34.0f, 1.0f / 34.0f));

	DGraphics::BeginScene(true, false, DColor(1,1,1,1.0f), m_shadowMap);
}

void DLight::EndRenderShadow()
{
	DGraphics::EndScene(m_shadowMap);
	DGraphics::GlPopMatrix();

	DShader::SetGlobalTexture("g_shadowMap", m_shadowMap);
	DShader::SetGlobalMatrix("g_shadowView", m_view);
	DShader::SetGlobalMatrix("g_shadowProj", m_proj);
}
