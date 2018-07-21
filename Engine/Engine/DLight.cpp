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
	m_culler = 0;
}


DLight::~DLight()
{
}

void DLight::RenderShadow()
{
	float w = m_shadowMap->GetWidth();
	float h = m_shadowMap->GetHeight();
	//DGraphics::ResetViewPort();
	DGraphics::SetViewPort(0, 0, w, h);
	BeginRenderShadow();
	DGraphics::SetGlobalRenderShader(m_shadowShader);
	DScene::Draw(false, m_culler);
	
	EndRenderShadow();
	DGraphics::ClearGlobalRenderShader();
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

bool DLight::OnInit()
{
	DLightNode* lightNode = DSystem::GetSceneMgr()->GetCurrentScene()->GetLightNode();
	m_node = new DLightNode();
	m_node->light = this;

	m_culler = new DCuller();

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

	delete m_culler;
	m_culler = NULL;
}

void DLight::OnUpdate()
{
}

void DLight::OnFixedUpdate()
{
}

void DLight::BeginRenderShadow()
{
	//DVector3 bcenter, bsize;
	//GetCameraBounds(target, &bcenter, &bsize);

	/*DVector3 up, forward, right, position;
	m_Transform->GetUp(up);
	m_Transform->GetForward(forward);
	m_Transform->GetRight(right);
	m_Transform->GetPosition(position);

	position = position + up*bcenter.y + forward*bcenter.z + right*bcenter.x;

	DVector3 lookAt = position + forward;

	DMatrix4x4::LookAt(&m_view, position, lookAt, up);

	float fparam = bsize.z * 2;
	DMatrix4x4::Ortho(&m_proj, bsize.x * 2, bsize.y * 2, 0, fparam);*/

	if (m_transform->IsMatrixWillChange())
	{
		DVector3 up, lookAt, position;
		m_transform->GetUp(up);
		m_transform->GetPosition(position);
		m_transform->GetForward(lookAt);

		DShader::SetGlobalVector3(D_LIGHT_DIR, lookAt);

		lookAt = position + lookAt;

		DMatrix4x4::LookAt(&m_view, position, lookAt, up);
	}

	if (m_isProjChanged)
	{
		m_isProjChanged = false;
		DMatrix4x4::Ortho(&m_proj, m_size, m_size, 0.0f, m_far);
	}
	
	

	DGraphics::GlPushMatrix();
	DGraphics::GlLoadIndentity();
	DGraphics::GlMultiMatrix(m_view);
	DGraphics::GlLoadProjectionMatrix(m_proj);

	DShader::SetGlobalVector4(D_PARAMS_SHADOW, DVector4(1.0f, 0.0f, m_far, 1.0f / m_far));

	DGraphics::BeginScene(true, false, true, DColor(1,1,1,1.0f), m_shadowMap);
}

void DLight::EndRenderShadow()
{
	DGraphics::EndScene(m_shadowMap);
	DGraphics::GlPopMatrix();

	DShader::SetGlobalTexture(D_TEXTURE_SHADOW, m_shadowMap);
	DShader::SetGlobalMatrix(D_MATRIX_SHADOW_V, m_view);
	DShader::SetGlobalMatrix(D_MATRIX_SHADOW_P, m_proj);
}

//void DLight::GetCameraBounds(DCamera * cam, DVector3 * outCenter, DVector3 * outSize)
//{
//	DTransform* transform = cam->GetTransform();
//	DVector3 camPos,camUp,camForward,camRight;
//	transform->GetPosition(camPos);
//	transform->GetUp(camUp);
//	transform->GetForward(camForward);
//	transform->GetRight(camRight);
//
//	DVector3 max, min;
//
//	float f = min(m_far, cam->GetFar());
//	if (cam->IsOrthographic())
//	{
//		float w = cam->GetOrthoSize()*cam->GetAspect();
//		float h = cam->GetOrthoSize();
//		float n = cam->GetNear();
//
//		DVector3 p0 = camPos - camUp*h - camRight*w + camForward*n;
//		DVector3 p1 = camPos + camUp*h - camRight*w + camForward*n;
//		DVector3 p2 = camPos + camUp*h + camRight*w + camForward*n;
//		DVector3 p3 = camPos - camUp*h + camRight*w + camForward*n;
//
//		DVector3 p4 = camPos - camUp*h - camRight*w + camForward*f;
//		DVector3 p5 = camPos + camUp*h - camRight*w + camForward*f;
//		DVector3 p6 = camPos + camUp*h + camRight*w + camForward*f;
//		DVector3 p7 = camPos - camUp*h + camRight*w + camForward*f;
//
//		DVector3 r0, r1, r2, r3, r4, r5, r6, r7;
//
//		m_Transform->TransformPointToLocal(p0, r0);
//		m_Transform->TransformPointToLocal(p1, r1);
//		m_Transform->TransformPointToLocal(p2, r2);
//		m_Transform->TransformPointToLocal(p3, r3);
//		m_Transform->TransformPointToLocal(p4, r4);
//		m_Transform->TransformPointToLocal(p5, r5);
//		m_Transform->TransformPointToLocal(p6, r6);
//		m_Transform->TransformPointToLocal(p7, r7);
//
//		max = r0, min = r0;
//		DVector3::Max(max, r1, max);
//		DVector3::Max(max, r2, max);
//		DVector3::Max(max, r3, max);
//		DVector3::Max(max, r4, max);
//		DVector3::Max(max, r5, max);
//		DVector3::Max(max, r6, max);
//		DVector3::Max(max, r7, max);
//
//		DVector3::Min(min, r1, min);
//		DVector3::Min(min, r2, min);
//		DVector3::Min(min, r3, min);
//		DVector3::Min(min, r4, min);
//		DVector3::Min(min, r5, min);
//		DVector3::Min(min, r6, min);
//		DVector3::Min(min, r7, min);
//	}
//	else
//	{
//		float h = tan(cam->GetFieldOfView()*0.5f)*f;
//		float w = h*cam->GetAspect();
//
//		DVector3 p0 = camPos;
//		DVector3 p1 = camPos - camUp*h - camRight*w + camForward*f;
//		DVector3 p2 = camPos + camUp*h - camRight*w + camForward*f;
//		DVector3 p3 = camPos + camUp*h + camRight*w + camForward*f;
//		DVector3 p4 = camPos - camUp*h + camRight*w + camForward*f;
//
//		DVector3 r0, r1, r2, r3, r4;
//
//		m_Transform->TransformPointToLocal(p0, r0);
//		m_Transform->TransformPointToLocal(p1, r1);
//		m_Transform->TransformPointToLocal(p2, r2);
//		m_Transform->TransformPointToLocal(p3, r3);
//		m_Transform->TransformPointToLocal(p4, r4);
//
//		max = r0, min = r0;
//		DVector3::Max(max, r1, max);
//		DVector3::Max(max, r2, max);
//		DVector3::Max(max, r3, max);
//		DVector3::Max(max, r4, max);
//
//		DVector3::Min(min, r1, min);
//		DVector3::Min(min, r2, min);
//		DVector3::Min(min, r3, min);
//		DVector3::Min(min, r4, min);
//	}
//
//	outSize->x = (max.x - min.x)*0.5f;
//	outSize->y = (max.y - min.y)*0.5f;
//	outSize->z = (max.z - min.z)*0.5f;
//
//	outCenter->x = min.x + outSize->x;
//	outCenter->y = min.y + outSize->y;
//	outCenter->z = min.z + outSize->z;
//}
