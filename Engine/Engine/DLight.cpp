#include "DLight.h"
#include "DShader.h"
#include "DSystem.h"

DLight::DLight() : DSceneObject()
{
	m_shadowMap = 0;
	m_shadowShader = 0;
	m_far = 100.0f;
	m_size = 30.0f;
	m_isProjChanged = false;

	m_node = 0;
	m_layerMask = D_LAYERMASK_DEFAULT;

	m_isShadowActive = true;
}


DLight::~DLight()
{
}

void DLight::RenderShadow()
{
	float w = m_shadowMap->GetWidth();
	float h = m_shadowMap->GetHeight();
	//DGraphics::ResetViewPort();

	ApplyLightDirParam();

	if (m_isShadowActive)
	{
		DGraphics::SetViewPort(0, 0, w, h);
		BeginRenderShadow();
		DGraphics::SetGlobalRenderShader(m_shadowShader);
		DScene::Draw(false, m_layerMask);

		EndRenderShadow();
		DGraphics::ClearGlobalRenderShader();
	}
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

float DLight::GetFar()
{
	return m_far;
}

float DLight::GetSize()
{
	return m_size;
}

void DLight::SetFar(float farClipPlane)
{
	m_isProjChanged = true;
	m_far = farClipPlane;
}

void DLight::SetSize(float size)
{
	m_isProjChanged = true;
	m_size = size;
}

void DLight::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

void DLight::SetShadowActive(bool active)
{
	m_isShadowActive = active;
}

bool DLight::GetShadowActive()
{
	return m_isShadowActive;
}

void DLight::SetLayerMask(DLAYER layerMask)
{
	m_layerMask = layerMask;
}

DLAYER DLight::GetLayerMask()
{
	return m_layerMask;
}

void DLight::AddLayer(DLAYER layer)
{
	m_layerMask |= layer;
}

void DLight::RemoveLayer(DLAYER layer)
{
	if ((m_layerMask&layer) != 0)
		m_layerMask ^= layer;
}

bool DLight::IsLayerVisible(DLAYER layer)
{
	return (m_layerMask&layer) != 0;
}

bool DLight::OnInit()
{
	DLightNode* lightNode = DSystem::GetSceneMgr()->GetCurrentScene()->GetLightNode();
	m_node = new DLightNode();
	m_node->light = this;

	if (lightNode == NULL)
		lightNode = m_node;
	else {
		lightNode->next = m_node;
		m_node->pre = lightNode;
	}
	DSystem::GetSceneMgr()->GetCurrentScene()->SetLightNode(lightNode);
	//float w, h;
	//DSystem::GetGraphicsMgr()->GetResolution(w, h);
	m_shadowMap = DRenderTexture::Create(1024, 1024);
	m_shadowShader = DShader::Create("../Res/shadow.shader");

	DMatrix4x4::Ortho(&m_proj, m_size, m_size, 0.0f, m_far);
	//DMatrix4x4::Perspective(&m_proj, D_PI / 3.0f, 1.0f, 0.03f, 34.0f);
	return true;
}

void DLight::OnDestroy()
{
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

	if (m_node != NULL)
	{
		if (m_node->pre == NULL)
		{
			DSystem::GetSceneMgr()->GetCurrentScene()->SetLightNode(m_node->next);
		}
		else
		{
			m_node->pre->next = m_node->next;
		}
		m_node->pre = NULL;
		m_node->next = NULL;
		delete m_node;
		m_node = NULL;
	}

}

void DLight::OnUpdate()
{
}

void DLight::OnFixedUpdate()
{
}

void DLight::ApplyLightDirParam()
{
	if (m_transform->IsMatrixWillChange())
	{
		DVector3 up, lookAt, position;
		m_transform->GetUp(up);
		m_transform->GetPosition(position);
		m_transform->GetForward(lookAt);

		DShader::SetGlobalVector3(D_SC_LIGHT_DIR, lookAt);

		lookAt = position + lookAt;

		DMatrix4x4::LookAt(&m_view, position, lookAt, up);
	}
}

void DLight::BeginRenderShadow()
{

	if (m_isProjChanged)
	{
		m_isProjChanged = false;
		DMatrix4x4::Ortho(&m_proj, m_size, m_size, 0.0f, m_far);
	}
	
	

	DGraphics::GlPushMatrix();
	DGraphics::GlLoadIndentity();
	DGraphics::GlMultiMatrix(m_view);
	DGraphics::GlLoadProjectionMatrix(m_proj);

	DShader::SetGlobalVector4(D_SC_PARAMS_SHADOW, DVector4(1.0f, 0.0f, m_far, 1.0f / m_far));

	DGraphics::BeginScene(true, false, true, DColor(1,1,1,1.0f), m_shadowMap);
}

void DLight::EndRenderShadow()
{
	DGraphics::EndScene(m_shadowMap);
	DGraphics::GlPopMatrix();

	DShader::SetGlobalTexture(D_SC_TEXTURE_SHADOW, m_shadowMap);
	DShader::SetGlobalMatrix(D_SC_MATRIX_SHADOW_V, m_view);
	DShader::SetGlobalMatrix(D_SC_MATRIX_SHADOW_P, m_proj);
}
