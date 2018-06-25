#include "DCamera.h"
#include "DSystem.h"

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
	m_aspect = width / height;
	m_ortho = false;
	m_isProjectionChanged = true;
	//m_position = D3DXVECTOR3(2.178069f, 3.102766f, -0.4222083f);

}


DCamera::~DCamera()
{
}

void DCamera::BeginRender()
{
	//context->RSSetViewports(1, &m_viewPort);
	sCurrent = this;

	if (m_isProjectionChanged)
	{
		if (m_ortho)
		{
			//d3dxmatrixor
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
}

void DCamera::EndRender()
{
	sCurrent = NULL;
}

void DCamera::RenderFilter()
{
	if (m_filter != NULL)
	{
		FLOAT width, height;

		DSystem::GetGraphicsMgr()->GetResolution(width, height);
		m_filter->Render(width, height);
	}
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

void DCamera::SetFilter(DCameraFilter * filter)
{
	m_filter = filter;
}

void DCamera::ClearFilter()
{
	m_filter = NULL;
}

void DCamera::GetCurrentCamera(DCamera ** cam)
{
	(*cam) = sCurrent;
}
