#include "stdafx.h"
#include "TestFilter.h"
#include "DSystem.h"
#include "TestResDefine.h"

TestFilter::TestFilter()
{
	m_shader = DRes::Load<DShader>(DEFAULT_GROUP, PEFFECT_SHADER);
	m_mat = new DMaterial(m_shader);
}


TestFilter::~TestFilter()
{
}

void TestFilter::Render(DRenderTexture * screenTexture)
{
	DGraphics::DrawTexture(screenTexture, m_mat);
}

void TestFilter::Release()
{
	if (m_shader != NULL)
	{
		m_shader->Destroy();
		delete m_shader;
		m_shader = NULL;
	}
	if (m_mat != NULL)
	{
		m_mat->Destroy();
		delete m_mat;
		m_mat = NULL;
	}
}
