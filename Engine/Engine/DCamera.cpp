#include "DCamera.h"
#include "DSystem.h"
#include "DScene.h"

DCamera::DCamera()
{
	m_filter = 0;
	m_fieldOfView = D_PI / 3.0f;;
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
	m_viewPort.width = width;
	m_viewPort.height = height;
	m_viewPort.x = 0;
	m_viewPort.y = 0;
	m_aspect = width / height;
	m_ortho = false;
	m_isProjectionChanged = true;
	//m_position = D3DXVECTOR3(2.178069f, 3.102766f, -0.4222083f);
	m_backgroundColor = dcol_blue;
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

void DCamera::Render()
{
	DGraphics::SetViewPort(m_viewPort);
	BeginRender();
	DScene::Draw(true);
	EndRender();
}

void DCamera::Render(DShader * replaceShader)
{
	/*DGraphics::SetViewPort(m_viewPort);
	BeginRender();
	DScene::Draw(true, replaceShader);
	EndRender();*/
}

void DCamera::Init()
{
	DSceneObject::Init();

}

void DCamera::Destroy()
{
	DSceneObject::Destroy();
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

void DCamera::GetCurrentCamera(DCamera ** cam)
{
	(*cam) = sCurrent;
}

void DCamera::BeginRender()
{
	//context->RSSetViewports(1, &m_viewPort);
	sCurrent = this;

	if (m_isProjectionChanged)
	{
		if (m_ortho)
		{
			DMatrix4x4::Ortho(&m_projection, m_aspect*m_orthoSize, m_orthoSize, m_near, m_far);
		}
		else
		{
			DMatrix4x4::Perspective(&m_projection, m_fieldOfView, m_aspect, m_near, m_far);
		}
		m_isProjectionChanged = false;
	}
	if (m_Transform->IsMatrixWillChange())
	{
		DVector3 up, lookAt, position;
		m_Transform->GetForward(lookAt);
		m_Transform->GetUp(up);
		m_Transform->GetPosition(position);

		lookAt = position + lookAt;

		DMatrix4x4::LookAt(&m_viewMatrix, position, lookAt, up);
	}

	DGraphics::GlLoadIndentity();
	DGraphics::GlMultiMatrix(m_viewMatrix);
	DGraphics::GlLoadProjectionMatrix(m_projection);

	if (m_renderTexture != NULL)
	{
		DGraphics::BeginScene(true, false, m_backgroundColor, m_renderTexture);
		//DGraphics::SetRenderTarget(m_renderTexture);
		//DGraphics::ClearRenderTarget(m_renderTexture, true, false, DColor(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else
	{
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