#include "DCameraFilter.h"

DCameraFilter::DCameraFilter()
{
	m_shader = DShader::Create(L"../Res/texture.vs", L"../Res/texture.ps");
	m_tex = DTexture2D::Create(L"../Res/check.jpg");
	m_material = new DMaterial(m_shader);
	m_material->SetTexture("shaderTexture", m_tex);
	m_material->SetZWrite(false);
	//m_material->SetCullMode(DCullMode_Off);
	m_material->SetZTest(DRSCompareFunc_Always);
}


DCameraFilter::~DCameraFilter()
{
}

void DCameraFilter::Render(float screenWidth, float screenHeight)
{
	if (m_mesh == NULL)
	{
		InitOutputPlane(screenWidth, screenHeight);
	}
	DMatrix4x4 world, proj;
	DMatrix4x4::Identity(&world);
	DMatrix4x4 view = DMatrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 1.0f);
	DMatrix4x4::Ortho(&proj, screenWidth, screenHeight, -100.0f, 100.0f);

	m_material->SetMatrix("worldMatrix", world);
	m_material->SetMatrix("viewMatrix", view);
	m_material->SetMatrix("projectionMatrix", proj);

	m_material->Apply();

	m_mesh->Draw();
}

void DCameraFilter::Release()
{
	if (m_mesh != NULL)
	{
		m_mesh->Destroy();
		delete m_mesh;
		m_mesh = NULL;
	}
	if (m_shader != NULL)
	{
		m_shader->Destroy();
		delete m_shader;
		m_shader = NULL;
	}
	if (m_tex != NULL)
	{
		m_tex->Destroy();
		delete m_tex;
		m_tex = NULL;
	}
	if (m_material != NULL)
	{
		m_material->Destroy();
		delete m_material;
		m_material = NULL;
	}
}

void DCameraFilter::InitOutputPlane(float screenWidth, float screenHeight)
{
	DMeshBufferDesc desc;

	int dataSize = sizeof(float) * 5;
	int vertexCount = 4;
	int indexCount = 6;
	int bufferLength = 5;

	float* buffer;
	unsigned long* indexBuffer;

	buffer = new float[vertexCount * 5];
	indexBuffer = new unsigned long[indexCount];

	buffer[0] = -0.5f*screenWidth; buffer[1] = -0.5f*screenHeight; buffer[2] = 0.0f; buffer[3] = 0.0f; buffer[4] = 0.0f;
	buffer[5] = -0.5f*screenWidth; buffer[6] = 0.5f*screenHeight; buffer[7] = 0.0f; buffer[8] = 0.0f; buffer[9] = 1.0f;
	buffer[10] = 0.5f*screenWidth; buffer[11] = 0.5f*screenHeight; buffer[12] = 0.0f; buffer[13] = 1.0f; buffer[14] = 1.0f;
	buffer[15] = 0.5f*screenWidth; buffer[16] = -0.5f*screenHeight; buffer[17] = 0.0f; buffer[18] = 1.0f; buffer[19] = 0.0f;
	
	indexBuffer[0] = 0; indexBuffer[1] = 1; indexBuffer[2] = 2;
	indexBuffer[3] = 0; indexBuffer[4] = 2; indexBuffer[5] = 3;

	desc.dataSize = dataSize;
	desc.dataCount = bufferLength;
	desc.indexCount = indexCount;
	desc.vertexCount = vertexCount;
	desc.indices = indexBuffer;
	desc.vertices = buffer;

	m_mesh = DMesh::Create(&desc);
}
