#include "DCamera.h"
#include "DSystem.h"

DCamera::DCamera()
{

	m_fieldOfView = (float)D3DX_PI / 3.0f;;
	m_near = 0.3f;
	m_far = 1000.0f;
	m_orthoSize = 5.0f;
	m_viewPort = D3D11_VIEWPORT();
	FLOAT width , height;
	
	DSystem::GetGraphicsCore()->GetResolution(width, height);
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;
	m_aspect = width/height;
	m_ortho = false;
	m_isProjectionChanged = true;
	//m_position = D3DXVECTOR3(2.178069f, 3.102766f, -0.4222083f);

}


DCamera::~DCamera()
{
}

void DCamera::BeginRender(ID3D11DeviceContext* context)
{
	context->RSSetViewports(1, &m_viewPort);
	sCurrent = this;

	if (m_isProjectionChanged)
	{
		if (m_ortho)
		{
			//d3dxmatrixor
		}
		else
		{
			D3DXMatrixPerspectiveFovLH(&m_projection, m_fieldOfView, m_aspect, m_near, m_far);
		}
		m_isProjectionChanged = false;
	}
	if (m_Transform->IsMatrixWillChange()) 
	{
		D3DXVECTOR3 up, lookAt, position;
		m_Transform->GetForward(lookAt);
		m_Transform->GetUp(up);
		m_Transform->GetPosition(position);

		lookAt = position + lookAt;

		D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	}
}

void DCamera::EndRender()
{
	sCurrent = NULL;
}

void DCamera::Init()
{
	DSceneObject::Init();
	
	//float yaw, pitch, roll;
	//D3DXVECTOR3 up, lookAt;
	//D3DXMATRIX rotationMatrix;

	//up.x = 0.165f;
	//up.y = 0.9669f;
	//up.z = -0.1946f;

	//lookAt.x = 0.6253f;
	//lookAt.y = -0.2552f;
	//lookAt.z = -0.7375f;

	//pitch = 0 * 0.0174532925f;
	//yaw = 0 * 0.0174532925f;
	//roll = 0 * 0.0174532925f;

	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	//lookAt = m_position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	//D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &lookAt, &up);


}

void DCamera::Destroy()
{
	DSceneObject::Destroy();
}

void DCamera::GetViewMatrix(D3DXMATRIX& mOut)
{
	mOut = m_viewMatrix;
}

void DCamera::GetProjection(D3DXMATRIX& mOut)
{
	mOut = m_projection;
}

float DCamera::GetFieldOfView()
{
	return m_fieldOfView;
}

float DCamera::GetNear()
{
	return m_near;
}

float DCamera::GetFar()
{
	return m_far;
}

float DCamera::GetAspect()
{
	return m_aspect;
}

float DCamera::GetOrthoSize()
{
	return m_orthoSize;
}

bool DCamera::IsOrthographic()
{
	return m_ortho;
}

void DCamera::SetFieldOfView(float fieldOfView)
{
	if (!IS_FLOAT_EQUAL(m_fieldOfView, fieldOfView)) 
	{
		m_fieldOfView = fieldOfView;
		if(!m_ortho)
			m_isProjectionChanged = true;
	}
}

void DCamera::SetNear(float nearClip)
{
	if (!IS_FLOAT_EQUAL(m_near, nearClip))
	{
		m_near = nearClip;
		m_isProjectionChanged = true;
	}
}

void DCamera::SetFar(float farClip)
{
	if (!IS_FLOAT_EQUAL(m_far, farClip))
	{
		m_far = farClip;
		m_isProjectionChanged = true;
	}
}

void DCamera::SetAspect(float aspect)
{
	if (!IS_FLOAT_EQUAL(m_aspect, aspect))
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
		if(m_ortho)
			m_isProjectionChanged = true;
	}
}

void DCamera::GetCurrentCamera(DCamera ** cam)
{
	(*cam) = sCurrent;
}
