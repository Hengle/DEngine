#include "DCamera.h"
#include "DSystem.h"
#include "DScene.h"

DCamera::DCamera() : DSceneObject()
{
	m_filter = 0;
	m_fieldOfView = D_PI / 3.0f;
	m_near = 0.3f;
	m_far = 1000.0f;
	m_orthoSize = 5.0f;
	FLOAT width, height;

	DSystem::GetGraphicsMgr()->GetResolution(width, height);
	/*m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;*/
	m_viewPort.width = 1.0f;
	m_viewPort.height = 1.0f;
	m_viewPort.x = 0;
	m_viewPort.y = 0;
	m_aspect = width / height;
	m_ortho = false;
	m_isProjectionChanged = true;
	//m_position = D3DXVECTOR3(2.178069f, 3.102766f, -0.4222083f);
	m_backgroundColor = dcol_blue;
	m_sortOrder = 0;
	m_node = 0;
}


DCamera::~DCamera()
{
}

void DCamera::RenderFilter()
{
	if (m_filter != NULL && m_renderTexture != NULL)
	{
		//FLOAT width, height;

		DGraphics::BeginScene(true, false, m_backgroundColor);
		//DGraphics::Clear(true, false, DColor(0.0f, 0.0f, 1.0f, 1.0f));
		//DSystem::GetGraphicsMgr()->GetResolution(width, height);
		m_filter->Render(m_renderTexture);
		DGraphics::EndScene();
	}
}

void DCamera::GetViewMatrix(DMatrix4x4& mOut) const
{
	mOut = m_viewMatrix;
}

void DCamera::GetProjection(DMatrix4x4& mOut)const
{
	mOut = m_projection;
}

void DCamera::GetBackgroundColor(DColor & color) const
{
	color = m_backgroundColor;
}

float DCamera::GetFieldOfView()const
{
	return m_fieldOfView;
}

float DCamera::GetNear()const
{
	return m_near;
}

float DCamera::GetFar()const
{
	return m_far;
}

float DCamera::GetAspect()const
{
	return m_aspect;
}

float DCamera::GetOrthoSize()const
{
	return m_orthoSize;
}

void DCamera::GetViewPort(DRect & rect) const
{
	rect = m_viewPort;
}

int DCamera::GetSortOrder() const
{
	return m_sortOrder;
}

ICameraFilter * DCamera::GetFilter() const
{
	return m_filter;
}

DMaterial * DCamera::GetSkuBox() const
{
	return m_skyBoxMaterial;
}

bool DCamera::IsOrthographic()const
{
	return m_ortho;
}

void DCamera::SetFieldOfView(float fieldOfView)
{
	if (!(IS_FLOAT_EQUAL(m_fieldOfView, fieldOfView)))
	{
		m_fieldOfView = fieldOfView;
		if (!m_ortho)
			m_isProjectionChanged = true;
	}
}

void DCamera::SetNear(float nearClip)
{
	if (!(IS_FLOAT_EQUAL(m_near, nearClip)))
	{
		m_near = nearClip;
		m_isProjectionChanged = true;
	}
}

void DCamera::SetFar(float farClip)
{
	if (!(IS_FLOAT_EQUAL(m_far, farClip)))
	{
		m_far = farClip;
		m_isProjectionChanged = true;
	}
}

void DCamera::SetAspect(float aspect)
{
	if (!(IS_FLOAT_EQUAL(m_aspect, aspect)))
	{
		m_aspect = aspect;
		m_isProjectionChanged = true;
	}
}

void DCamera::SetOrthographic(bool ortho)
{
	if (m_ortho != ortho)
	{
		m_ortho = ortho;
		m_isProjectionChanged = true;
	}
}

void DCamera::SetOrthoSize(float size)
{
	if (m_orthoSize != size)
	{
		m_orthoSize = size;
		if (m_ortho)
			m_isProjectionChanged = true;
	}
}

void DCamera::SetFilter(ICameraFilter * filter)
{
	m_filter = filter;
}

void DCamera::SetBackgroundColor(DColor & color)
{
	m_backgroundColor = color;
}

void DCamera::SetSkyBox(DMaterial * skyBoxMaterial)
{
	m_skyBoxMaterial = skyBoxMaterial;
}

void DCamera::SetViewPort(DRect & viewPort)
{
	m_viewPort = viewPort;
}

void DCamera::SetSortOrder(int sortOrder)
{
	m_sortOrder = sortOrder;
	if (m_node->pre != NULL && m_node->pre->camera->GetSortOrder() > m_sortOrder)
	{

	}
	else if(m_node->next != NULL && m_node->next->camera->GetSortOrder() < m_sortOrder)
	{

	}
}

void DCamera::ClearSkyBox()
{
	m_skyBoxMaterial = NULL;
}

void DCamera::ClearFilter()
{
	m_filter = NULL;
}

DRenderTexture * DCamera::GetRenderTexture()
{
	return m_renderTexture;
}

void DCamera::SetRenderTexture(DRenderTexture * renderTexture)
{
	m_renderTexture = renderTexture;
}

void DCamera::SetReplaceShader(DShader * replacement)
{
	m_replacementShader = replacement;
}

void DCamera::ResetReplaceShader()
{
	m_replacementShader = NULL;
}

void DCamera::GetCurrentCamera(DCamera ** cam)
{
	(*cam) = sCurrent;
}

bool DCamera::OnInit()
{
	DCameraNode* camNode = DSystem::GetSceneMgr()->GetCurrentScene()->GetCameraNode();
	m_node = new DCameraNode();
	m_node->camera = this;
	if (camNode == NULL)
		camNode = m_node;
	else {
		if (camNode->camera->GetSortOrder() >= m_sortOrder)
		{
			m_node->next = camNode;
			camNode->pre = m_node;
			camNode = m_node;
		}
		else
		{
			DCameraNode* node = camNode;
			while (node != NULL && node->camera->GetSortOrder() < m_sortOrder)
			{
				if (node->next == NULL)
				{
					node->next = m_node;
					m_node->pre = node;
					break;
				}
				if (node->next->camera->GetSortOrder() >= m_sortOrder)
				{
					m_node->next = node->next;
					node->next->pre = m_node;
					node->next = m_node;
					m_node->pre = node;
					break;
				}
				node = node->next;
			}
		}
	}
	DSystem::GetSceneMgr()->GetCurrentScene()->SetCameraNode(camNode);
	return true;
}

void DCamera::OnDestroy()
{
	m_renderTexture = NULL;
	m_replacementShader = NULL;
	m_skyBoxMaterial = NULL;
	m_filter = NULL;

	if (m_node != NULL)
	{
		if (m_node->pre == NULL)
		{
			DSystem::GetSceneMgr()->GetCurrentScene()->SetCameraNode(m_node->next);
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

void DCamera::OnUpdate()
{
}

void DCamera::OnFixedUpdate()
{
}

void DCamera::OnRender()
{
	BeginRender();
	if (m_replacementShader != NULL)
		DScene::Draw(true, m_replacementShader);
	else
		DScene::Draw(true);
	EndRender();
}

void DCamera::ForwardMoveCameraNode()
{
	if (m_node->pre != NULL && m_node->pre->camera->GetSortOrder() > m_sortOrder)
	{
		DCameraNode* pre = m_node->pre;
		pre->next = m_node->next;
		m_node->pre = pre->pre;
		pre->pre = m_node;
		ForwardMoveCameraNode();
	}
	else if (m_node->pre == NULL)
	{
		DSystem::GetSceneMgr()->GetCurrentScene()->SetCameraNode(m_node);
	}
}

void DCamera::BackwardMoveCameraNode()
{
	if (m_node->next != NULL && m_node->next->camera->GetSortOrder() < m_sortOrder)
	{
		DCameraNode* next = m_node->next;
		next->pre = m_node->pre;
		m_node->next = next->next;
		next->next = m_node;
		BackwardMoveCameraNode();
	}
}

void DCamera::BeginRender()
{
	//context->RSSetViewports(1, &m_viewPort);
	sCurrent = this;

	if (m_isProjectionChanged)
	{
		if (m_ortho)
		{
			DMatrix4x4::Ortho(&m_projection, m_aspect*m_orthoSize*2, m_orthoSize*2, m_near, m_far);
		}
		else
		{
			DMatrix4x4::Perspective(&m_projection, m_fieldOfView, m_aspect, m_near, m_far);
		}
		m_isProjectionChanged = false;
	}
	if (m_transform->IsMatrixWillChange())
	{
		DVector3 up, lookAt, position;
		m_transform->GetForward(lookAt);
		m_transform->GetUp(up);
		m_transform->GetPosition(position);

		lookAt = position + lookAt;

		DMatrix4x4::LookAt(&m_viewMatrix, position, lookAt, up);
	}

	DGraphics::GlLoadIndentity();
	DGraphics::GlMultiMatrix(m_viewMatrix);
	DGraphics::GlLoadProjectionMatrix(m_projection);

	if (m_renderTexture != NULL)
	{
		float rtw, rth;
		rtw = m_renderTexture->GetWidth();
		rth = m_renderTexture->GetHeight();
		float vx = rtw*m_viewPort.x;
		float vy = rth*m_viewPort.y;
		float vw = rtw*m_viewPort.width;
		float vh = rth*m_viewPort.height;
		DGraphics::SetViewPort(vx, vy, vw, vh);
		DGraphics::BeginScene(true, false, m_backgroundColor, m_renderTexture);
		//DGraphics::SetRenderTarget(m_renderTexture);
		//DGraphics::ClearRenderTarget(m_renderTexture, true, false, DColor(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else
	{
		float screenw, screenh;
		DSystem::GetGraphicsMgr()->GetResolution(screenw, screenh);
		float vx = screenw*m_viewPort.x;
		float vy = screenh*m_viewPort.y;
		float vw = screenw*m_viewPort.width;
		float vh = screenh*m_viewPort.height;
		DGraphics::SetViewPort(vx, vy, vw, vh);
		DGraphics::BeginScene(true, false, m_backgroundColor);
		//DGraphics::SetDefaultRenderTarget();
		//DGraphics::Clear(true, false, DColor(0.0f, 0.0f, 1.0f, 1.0f));
	}

	if (m_skyBoxMaterial != NULL)
	{
		DGraphics::DrawSkyBox(m_skyBoxMaterial, this);
	}
}

void DCamera::EndRender()
{
	if (m_renderTexture != NULL)
	{
		DGraphics::EndScene(m_renderTexture);
	}
	else
	{
		DGraphics::EndScene();
	}
	sCurrent = NULL;
}