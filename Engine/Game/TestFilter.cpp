#include "stdafx.h"
#include "TestFilter.h"
#include "DSystem.h"
#include "TestResDefine.h"

TestFilter::TestFilter()
{
	//m_shader = DRes::Load<DShader>(DEFAULT_GROUP, PEFFECT_SHADER);
	//m_mat = new DMaterial(m_shader);
	m_mat = DRes::Load<DMaterial>(DEFAULT_GROUP, PEFFECT_MAT);
	//m_mat->SetFloat("offset", 0.008f);

	m_dst = DRenderTexture::Create(1024.0f, 768.0f);
	//testshadow = DShader::Create("../Res/testshadow.shader");
	//m_mat = new DMaterial(testshadow);
}


TestFilter::~TestFilter()
{
}

void TestFilter::Render(DRenderTexture * screenTexture)
{
	m_mat->SetVector2("offset", DVector2(0.008f, 0.008f));
	/*DGraphics::Blit(screenTexture, m_dst, m_mat);
	m_mat->SetVector2("offset", DVector2(-0.008f, -0.008f));
	DGraphics::Blit(m_dst, screenTexture, m_mat);
	m_mat->SetVector2("offset", DVector2(0.008f, 0.008f));
	DGraphics::Blit(screenTexture, m_dst, m_mat);
	m_mat->SetVector2("offset", DVector2(-0.008f, -0.008f));*/
	DGraphics::Blit(screenTexture, m_mat);
	//DGraphics::Blit(m_dst, m_mat);
}

void TestFilter::Release()
{
	if (m_dst != NULL)
	{
		m_dst->Destroy();
		delete m_dst;
		m_dst = NULL;
	}
	if (m_mat != NULL)
	{
		m_mat->Destroy();
		delete m_mat;
		m_mat = NULL;
	}

}
