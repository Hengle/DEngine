#include "DGLCore.h"

DMeshRes::DMeshRes(int vertexUsage, bool dynamic)
{
	m_isSupported = false;
	m_isInitialized = false;
	m_vertexUsage = vertexUsage;
	m_indexCount = 0;
	m_vertexCount = 0;

	m_isDynamic = dynamic;

	m_dataCount = 3;
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD0))
	{
		//m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD1))
	{
		//m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV1 = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD2))
	{
		//m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV2 = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TEXCOORD3))
	{
		//m_dataSize += fsize * 2;
		m_dataCount += 2;
		m_hasUV3 = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_NORMAL))
	{
		//m_dataSize += fsize * 3;
		m_dataCount += 3;
		m_hasNormal = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_COLOR))
	{
		//m_dataSize += fsize * 4;
		m_dataCount += 4;
		m_hasColor = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_TANGENT))
	{
		//m_dataSize += fsize * 4;
		m_dataCount += 4;
		m_hasTangent = true;
	}
	if (vertexUsage & (1UL << DVertexUsage_BINORMAL))
	{
		//m_dataSize += fsize * 3;
		m_dataCount += 3;
		m_hasBinormal = true;
	}

	m_dataSize = sizeof(float) * m_dataCount;
}

//void DMeshRes::Init(DMeshBufferDesc * desc)
//{
//	m_isInitialized = OnInit(desc);
//}

void DMeshRes::Refresh(DMeshBufferDesc * desc)
{
	if (desc == NULL)
		return;
	if (desc->indexCount <= 0 || desc->vertexCount <= 0)
		return;
	if (desc->vertices == nullptr)
		return;
	if (desc->indices == nullptr)
		return;
	m_indexCount = desc->indexCount;
	m_vertexCount = desc->vertexCount;


	float* vertices = new float[m_dataCount * desc->vertexCount];
	int i, j;
	for (i = 0; i < desc->vertexCount; i++)
	{
		j = 0;
		vertices[i*m_dataCount + j] = desc->vertices[i * 3];
		vertices[i*m_dataCount + j + 1] = desc->vertices[i * 3 + 1];
		vertices[i*m_dataCount + j + 2] = desc->vertices[i * 3 + 2];
		j += 3;
		if (m_hasNormal)
		{
			vertices[i*m_dataCount + j] = desc->normals != 0 ? desc->normals[i * 3] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->normals != 0 ? desc->normals[i * 3 + 1] : 0.0f;
			vertices[i*m_dataCount + j + 2] = desc->normals != 0 ? desc->normals[i * 3 + 2] : 0.0f;
			j += 3;
		}
		if (m_hasColor)
		{
			vertices[i*m_dataCount + j] = desc->colors != 0 ? desc->colors[i * 4] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->colors != 0 ? desc->colors[i * 4 + 1] : 0.0f;
			vertices[i*m_dataCount + j + 2] = desc->colors != 0 ? desc->colors[i * 4 + 2] : 0.0f;
			vertices[i*m_dataCount + j + 3] = desc->colors != 0 ? desc->colors[i * 4 + 3] : 0.0f;
			j += 4;
		}
		if (m_hasUV)
		{
			vertices[i*m_dataCount + j] = desc->uvs != 0 ? desc->uvs[i * 2] : 0.0f;
			vertices[i*m_dataCount + j + 1] = desc->uvs != 0 ? desc->uvs[i * 2 + 1] : 0.0f;
			j += 2;
		}
	}

	if (!m_isInitialized)
	{
		m_isSupported = OnInit(vertices, desc->indices, desc->vertexCount, desc->indexCount);
		m_isInitialized = true;
	}
	else {
		if (m_isSupported)
			OnRefresh(vertices, desc->indices, desc->vertexCount, desc->indexCount);
	}

	delete[] vertices;
	vertices = 0;
}

void DMeshRes::Refresh(float * vertexbuffer, unsigned long * indexbuffer, int vertexCount, int indexCount)
{
	if (indexCount <= 0 || vertexCount <= 0)
		return;
	if (vertexbuffer == 0)
		return;
	if (indexbuffer == 0)
		return;
	m_indexCount = indexCount;
	m_vertexCount = vertexCount;

	if (!m_isInitialized)
	{
		m_isSupported = OnInit(vertexbuffer, indexbuffer, vertexCount, indexCount);
		m_isInitialized = true;
	}
	else {
		if (m_isSupported)
			OnRefresh(vertexbuffer, indexbuffer, vertexCount, indexCount);
	}
}

void DMeshRes::DrawPrimitive(DMeshTopology topology)
{
	if (m_isSupported)
		OnDraw(topology);
}

bool DMeshRes::IsInitialized()
{
	return m_isInitialized;
}

DShaderProgram::DShaderProgram()
{
	m_isInitialized = false;
	m_cbufferCount = 0;
	m_propertyCount = 0;
	m_vertexUsage = 0;
	m_resCount = 0;
}

//unsigned int DShaderRes::GetCBufferCount() const
//{
//	return m_cbufferCount;
//}

unsigned int DShaderProgram::GetPropertyCount() const
{
	return m_propertyCount;
}

//void DShaderRes::Init(WCHAR * vsfile, WCHAR * psfile)
//{
//	m_isInitialized = OnInit(vsfile, psfile);
//}

void DShaderProgram::Init(const char * content, char * funcName)
{
	m_isInitialized = OnInit(content, funcName);
}

unsigned int DShaderProgram::GetResCount() const
{
	return m_resCount;
}

void DShaderProgram::ApplyParams(std::map<std::string, float*>& params, std::map<std::string, float*>&gparams)
{
	if (m_isInitialized)
		OnApplyParams(params, gparams);
}

//void DShaderRes::ApplyParams(int cindex, int coffset, int csize, int stype, float * params)
//{
//	if (m_isInitialized)
//		OnApplyParams(cindex, coffset, csize, stype, params);
//}

void DShaderProgram::Draw()
{
	if (m_isInitialized)
		OnDraw();
}

bool DShaderProgram::IsInitialized()
{
	return m_isInitialized;
}

int DShaderProgram::GetVertexUsage()
{
	return m_vertexUsage;
}


DGLCore::DGLCore()
{
}


DGLCore::~DGLCore()
{
}

bool DGLCore::Init(int width, int height, bool, HWND)
{
	m_width = width;
	m_height = height;
	return true;
}

void DGLCore::GetResolution(float & width, float & height)
{
	width = m_width;
	height = m_height;
}
