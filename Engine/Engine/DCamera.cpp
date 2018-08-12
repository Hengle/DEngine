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

	//m_render = 0;

	m_layerMask = D_LAYERMASK_DEFAULT;
	m_clearFlags = DClearFlags_SkyBox;

	m_additionalTexture = 0U;
}


DCamera::~DCamera()
{
}

void DCamera::Render()
{
	RenderScreenTexture();
	RenderDepthTexture();

	RenderFilter();
}

void DCamera::RenderFilter()
{
	if (m_filter != NULL)
	{
		DRenderTexture* src = GetSourceTexture();
		//bool clearDepth = m_clearFlags != DClearFlags_DontClear;
		//bool clearColor = m_clearFlags != DClearFlags_Depth && m_clearFlags != DClearFlags_DontClear;
		//DGraphics::BeginScene(clearDepth, clearDepth, clearColor, m_backgroundColor);
		DRenderTexture* dst = m_filter->Render(src);
		//DGraphics::EndScene();

		if (m_renderTexture != NULL && dst != NULL)
		{
			DGraphics::Blit(dst, m_renderTexture);
		}
		else if(dst != NULL)
		{
			DGraphics::DrawTexture(dst);
		}
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
	throw "TODO 暂未实现重新排序";
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

DRenderTexture * DCamera::GetRenderTexture() const
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

void DCamera::SetClearFlags(DClearFlags clearFlags)
{
	m_clearFlags = clearFlags;
}

DClearFlags DCamera::GetClearFlags() const
{
	return m_clearFlags;
}

void DCamera::ScreenToWorldPoint(const DVector3 point, DVector3 * out) const
{
	ScreenToWorldPoint(point.x, point.z, point.z, out);
}

void DCamera::ScreenToWorldPoint(float x, float y, float z, DVector3 * out) const
{
	FLOAT width, height;

	DSystem::GetGraphicsMgr()->GetResolution(width, height);
	float pixx = m_viewPort.x * width;
	float pixy = m_viewPort.y * height;
	float pixw = m_viewPort.width * width;
	float pixh = m_viewPort.height * height;

	float vx = (x - pixx) / pixw;
	float vy = (y - pixy) / pixh;

	CameraPointToWorldPoint(vx, vy, z, out);
}

void DCamera::ScreenToViewportPoint(const DVector3 point, DVector3 * out) const
{
	ScreenToViewportPoint(point.x, point.y, point.z, out);
}

void DCamera::ScreenToViewportPoint(float x, float y, float z, DVector3 * out) const
{
	FLOAT width, height;

	DSystem::GetGraphicsMgr()->GetResolution(width, height);
	float pixx = m_viewPort.x * width;
	float pixy = m_viewPort.y * height;
	float pixw = m_viewPort.width * width;
	float pixh = m_viewPort.height * height;
	float vx = (x - pixx) / pixw;
	float vy = (y - pixy) / pixh;

	out->x = vx;
	out->y = vy;
	out->z = z;
}

void DCamera::ViewportToScreenPoint(const DVector3 point, DVector3 * out) const
{
	ViewportToScreenPoint(point.x, point.y, point.z, out);
}

void DCamera::ViewportToScreenPoint(float x, float y, float z, DVector3 * out) const
{
	FLOAT width, height;

	DSystem::GetGraphicsMgr()->GetResolution(width, height);
	float vx = (m_viewPort.x + m_viewPort.width*x)*width;
	float vy = (m_viewPort.y + m_viewPort.height * y) * height;

	out->x = vx;
	out->y = vy;
	out->z = z;
}

void DCamera::ViewportToWorldPoint(const DVector3 point, DVector3 * out) const
{
	ViewportToWorldPoint(point.x, point.y, point.z, out);
}

void DCamera::ViewportToWorldPoint(float x, float y, float z, DVector3 * out) const
{

	float vx = (m_viewPort.x + m_viewPort.width*x - m_viewPort.x) / m_viewPort.width;
	float vy = (m_viewPort.y + m_viewPort.height * y - m_viewPort.y) / m_viewPort.height;
	
	CameraPointToWorldPoint(vx, vy, z, out);
}

void DCamera::WorldToScreenPoint(const DVector3 point, DVector3 * out) const
{
	WorldToScreenPoint(point.x, point.y, point.z, out);
}

void DCamera::WorldToScreenPoint(float x, float y, float z, DVector3 * out) const
{
	DVector3 viewport;
	WorldToViewportPoint(x, y, z, &viewport);
	ViewportToScreenPoint(viewport, out);
}

void DCamera::WorldToViewportPoint(const DVector3 point, DVector3 * out) const
{
	WorldToViewportPoint(point.x, point.y, point.z, out);
}

void DCamera::WorldToViewportPoint(float x, float y, float z, DVector3 * out) const
{
	DVector3 right, up, forward, position;
	m_transform->GetRight(right);
	m_transform->GetUp(up);
	m_transform->GetForward(forward);
	m_transform->GetPosition(position);

	float localX = x - position.x;
	float localY = y - position.y;
	float localZ = z - position.z;
	localX = x*right.x + y*right.y + z*right.z;
	localY = x*up.x + y*up.y + z*up.z;
	localZ = x*forward.x + y*forward.y + z*forward.z;

	float vx, vy;
	if (m_ortho)
	{
		vx = localX / (m_orthoSize * m_aspect*0.5f);
		vy = localY / (m_orthoSize*0.5f);

	}
	else
	{
		float height = tanf(m_fieldOfView * 0.5f) * localZ;
		vx = localX / (height * m_aspect);
		vy = localY / height;
	}
	vx = vx * 0.5f + 0.5f;
	vy = vy * 0.5f + 0.5f;

	out->x = vx;
	out->y = vy;
	out->z = localZ;
}

void DCamera::ScreenPointToRay(const DVector3 point, DRay * out) const
{
	ScreenPointToRay(point.x, point.y, point.z, out);
}

void DCamera::ScreenPointToRay(float x, float y, float z, DRay * out) const
{
	FLOAT width, height;

	DSystem::GetGraphicsMgr()->GetResolution(width, height);
	float pixx = m_viewPort.x * width;
	float pixy = m_viewPort.y * height;
	float pixw = m_viewPort.width * width;
	float pixh = m_viewPort.height * height;

	float vx = (x - pixx) / pixw;
	float vy = (y - pixy) / pixh;
	
	CameraPointToRay(vx, vy, z, out);
}

void DCamera::ViewportPointToRay(const DVector3 point, DRay * out) const
{
	ViewportPointToRay(point.x, point.y, point.z, out);
}

void DCamera::ViewportPointToRay(float x, float y, float z, DRay * out) const
{
	float vx = (m_viewPort.x + m_viewPort.width*x - m_viewPort.x) / m_viewPort.width;
	float vy = (m_viewPort.y + m_viewPort.height * y - m_viewPort.y) / m_viewPort.height;
	CameraPointToRay(vx, vy, z, out);
}

void DCamera::SetAdditionalTextureActive(DCameraAdditionalTextureType type, bool active)
{
	if (active)
		m_additionalTexture |= type;
	else
	{
		if ((m_additionalTexture&type) != 0)
			m_additionalTexture ^= type;
	}
}

bool DCamera::IsAdditionalTextureActive(DCameraAdditionalTextureType type) const
{
	return ((m_additionalTexture&type) != 0);
}

void DCamera::SetLayerMask(DLAYER layerMask)
{
	m_layerMask = layerMask;
}

DLAYER DCamera::GetLayerMask() const
{
	return m_layerMask;
}

void DCamera::AddLayer(DLAYER layer)
{
	m_layerMask |= layer;
}

void DCamera::RemoveLayer(DLAYER layer)
{
	if ((m_layerMask&layer) != 0)
		m_layerMask ^= layer;
}

bool DCamera::IsLayerVisible(DLAYER layer) const
{
	return (m_layerMask&layer) != 0;
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

	if (m_sourceTexture != NULL)
	{
		m_sourceTexture->Destroy();
		delete m_sourceTexture;
		m_sourceTexture = NULL;
	}

	if (m_depthTexture != NULL)
	{
		m_depthTexture->Destroy();
		delete m_depthTexture;
		m_depthTexture = NULL;
	}

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

void DCamera::RenderDepthTexture()
{
	if (!IsAdditionalTextureActive(DCameraAdditional_Depth))
		return;
	DShader* depthShader = DRes::LoadInternal<DShader>(D_RES_SHADER_DEPTH);
	DRenderTexture* depthTexture = GetDepthTexture();
	BeginRender(depthTexture, true, true, DCOLOR_WHITE);

	DGraphics::SetGlobalRenderShader(depthShader);

	DScene::Draw(false, m_layerMask);

	EndRender(depthTexture);
	DGraphics::ClearGlobalRenderShader();

	DShader::SetGlobalTexture(D_SC_DEPTH_TEXTURE, depthTexture);
}

void DCamera::RenderScreenTexture()
{
	DRenderTexture* src = NULL;
	if (m_filter != NULL)
	{
		src = GetSourceTexture();
	}
	bool clearDepth = m_clearFlags != DClearFlags_DontClear;
	bool clearColor = m_clearFlags != DClearFlags_Depth && m_clearFlags != DClearFlags_DontClear;
	BeginRender(src, clearDepth, clearColor, m_backgroundColor);
	if (m_skyBoxMaterial != NULL && m_clearFlags == DClearFlags_SkyBox)
	{
		DGraphics::DrawSkyBox(m_skyBoxMaterial, this);
	}

	DGraphics::SetGlobalRenderShader(m_replacementShader);

	OnPreRender();

	DScene::Draw(true, m_layerMask);

	OnPostRender();


	EndRender(src);
	DGraphics::ClearGlobalRenderShader();
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

void DCamera::BeginRender(DRenderTexture* renderTexture, bool clearDepth, bool clearColor, DColor& color)
{
	
	sCurrent = this;

	RefreshCameraDirParam();

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

	if (renderTexture != NULL)
	{
		float rtw, rth;
		rtw = renderTexture->GetWidth();
		rth = renderTexture->GetHeight();
		float vx = rtw*m_viewPort.x;
		float vy = rth*m_viewPort.y;
		float vw = rtw*m_viewPort.width;
		float vh = rth*m_viewPort.height;
		DGraphics::SetViewPort(vx, vy, vw, vh);

		//bool clearDepth = m_clearFlags != DClearFlags_DontClear;
		//bool clearColor = m_clearFlags != DClearFlags_Depth && m_clearFlags != DClearFlags_DontClear;
		DGraphics::BeginScene(clearDepth, clearDepth, clearColor, color, renderTexture);
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

		//bool clearDepth = m_clearFlags != DClearFlags_DontClear;
		//bool clearColor = m_clearFlags != DClearFlags_Depth && m_clearFlags != DClearFlags_DontClear;
		DGraphics::BeginScene(clearDepth, clearDepth, clearColor, color);
	}

	/*if (m_skyBoxMaterial != NULL && m_clearFlags == DClearFlags_SkyBox)
	{
		DGraphics::DrawSkyBox(m_skyBoxMaterial, this);
	}*/
}

void DCamera::EndRender(DRenderTexture* renderTexture)
{
	if (renderTexture != NULL)
	{
		DGraphics::EndScene(renderTexture);
	}
	else
	{
		DGraphics::EndScene();
	}
	sCurrent = NULL;
}

void DCamera::RefreshCameraDirParam()
{
	if (m_ortho)
	{
		DVector3 forward;
		m_transform->GetForward(forward);
		DShader::SetGlobalVector4(D_SC_CAMERA_POS, DVector4(forward.x, forward.y, forward.z, 0.0f));
	}
	else
	{
		DVector3 pos;
		m_transform->GetPosition(pos);
		DShader::SetGlobalVector4(D_SC_CAMERA_POS, DVector4(pos.x, pos.y, pos.z, 1.0f));
	}
}

void DCamera::CameraPointToWorldPoint(float x, float y, float z, DVector3 * out) const
{
	x = x * 2.0f - 1.0f;
	y = y * 2.0f - 1.0f;

	if (m_ortho)
	{
		x = x*m_orthoSize*m_aspect;
		y = y*m_orthoSize;
	}
	else
	{
		float height = tanf(m_fieldOfView*0.5f)*m_near;
		x = x * height * m_aspect;
		y = y * height;
	}
	DVector3 right, up, forward, position;
	m_transform->GetRight(right);
	m_transform->GetUp(up);
	m_transform->GetForward(forward);
	m_transform->GetPosition(position);
	DVector3 dir = right*x + up*y + forward*z;

	out->x = position.x + dir.x;
	out->y = position.y + dir.y;
	out->z = position.z + dir.z;
}

void DCamera::CameraPointToRay(float x, float y, float z, DRay * out) const
{
	x = x * 2.0f - 1.0f;
	y = y * 2.0f - 1.0f;

	if (m_ortho)
	{
		x = x * m_orthoSize * m_aspect;
		y = y * m_orthoSize;
		DVector3 right, up, forward, position;
		m_transform->GetRight(right);
		m_transform->GetUp(up);
		m_transform->GetForward(forward);
		m_transform->GetPosition(position);
		DVector3 dir = right*x + up*y + forward*z;
		out->origin.x = position.x + dir.x;
		out->origin.y = position.y + dir.y;
		out->origin.z = position.z + dir.z;
		out->direction.x = forward.x;
		out->direction.y = forward.y;
		out->direction.z = forward.z;
	}
	else
	{
		float height = tanf(m_fieldOfView * 0.5f) * m_near;
		x = x * height * m_aspect;
		y = y * height;
		DVector3 right, up, forward, position;
		m_transform->GetRight(right);
		m_transform->GetUp(up);
		m_transform->GetForward(forward);
		m_transform->GetPosition(position);
		DVector3 dir = right*x + up*y + forward*z;
		out->origin.x = position.x + dir.x;
		out->origin.y = position.y + dir.y;
		out->origin.z = position.z + dir.z;
		dir.Normalize();
		out->direction.x = dir.x;
		out->direction.y = dir.y;
		out->direction.z = dir.z;
	}
}

DRenderTexture * DCamera::GetSourceTexture()
{
	if (m_sourceTexture == NULL)
	{
		float w, h;
		DSystem::GetGraphicsMgr()->GetResolution(w, h);
		m_sourceTexture = DRenderTexture::Create(w, h);
	}
	return m_sourceTexture;
}

DRenderTexture * DCamera::GetDepthTexture()
{
	if (m_depthTexture == NULL)
	{
		float w, h;
		DSystem::GetGraphicsMgr()->GetResolution(w, h);
		m_depthTexture = DRenderTexture::Create(w, h);
	}
	return m_depthTexture;
}
