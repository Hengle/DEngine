#include "DDisplayObject.h"
#include "DSystem.h"

DDisplayObject::DDisplayObject()
{
}

DDisplayObject::DDisplayObject(DMesh* mesh, DLightShader* shader, DTexture* texture, DTexture* decal, DLight* light)
{
	m_shader = shader;
	m_mesh = mesh;
	m_texture = texture;
	m_decal = decal;
	m_light = light;
}


DDisplayObject::~DDisplayObject()
{
}

void DDisplayObject::Render()
{
	if (m_mesh != NULL && m_shader != NULL)
	{
		D3DXMATRIX world, view, projection;
		m_mesh->Render(DSystem::GetGraphicsCore()->GetDeviceContext());
		DCamera* cur;
		DCamera::GetCurrentCamera(&cur);
		m_Transform->GetLocalToWorld(world);
		cur->GetViewMatrix(view);
		cur->GetProjection(projection);
		D3DXVECTOR3 lightDir;
		D3DXVECTOR4 lightCol;
		m_light->GetTransform()->GetForward(lightDir);
		m_light->GetColor(lightCol);
		m_shader->Render(DSystem::GetGraphicsCore()->GetDeviceContext(), m_mesh->GetIndexCount(), world, view, projection, m_texture->GetTexture(), m_decal->GetTexture(), lightDir, lightCol);
	}
}

void DDisplayObject::Init()
{
	DSceneObject::Init();
	if (m_shader != NULL) 
	{
		m_shader->Init(DSystem::GetGraphicsCore()->GetDevice());
	}
	if (m_mesh != NULL)
	{
		m_mesh->Init(DSystem::GetGraphicsCore()->GetDevice());
	}
	if (m_texture != NULL)
	{
		m_texture->Init(DSystem::GetGraphicsCore()->GetDevice());
	}
	if (m_decal != NULL)
	{
		m_decal->Init(DSystem::GetGraphicsCore()->GetDevice());
	}
	if (m_light != NULL)
	{
		m_light->Init();
	}
}

void DDisplayObject::Destroy()
{
	DSceneObject::Destroy();
	if (m_shader != NULL) {
		m_shader->Destroy();
		delete m_shader;
		m_shader = NULL;
	}
	if (m_mesh != NULL)
	{
		m_mesh->Destroy();
		delete m_mesh;
		m_mesh = NULL;
	}
	if (m_texture != NULL) 
	{
		m_texture->Destroy();
		delete m_texture;
		m_texture = NULL;
	}
	if (m_decal != NULL)
	{
		m_decal->Destroy();
		delete m_decal;
		m_decal = NULL;
	}
	if (m_light != NULL)
	{
		m_light->Destroy();
		delete m_light;
		m_light = NULL;
	}
}
