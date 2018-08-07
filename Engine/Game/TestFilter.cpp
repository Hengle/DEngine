#include "stdafx.h"
#include "TestFilter.h"
#include "DSystem.h"
#include "TestResDefine.h"

TestFilter::TestFilter()
{
	//m_shader = DRes::Load<DShader>(DEFAULT_GROUP, PEFFECT_SHADER);
	//m_mat = new DMaterial(m_shader);
	m_glowMat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3011);
	m_mixMat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3012);
	m_mat = DRes::Load<DMaterial>(DEFAULT_GROUP, PEFFECT_MAT);
	//m_mat->SetFloat("offset", 0.008f);

	m_dst = DRenderTexture::Create(256.0f, 256.0f);
	m_dst2 = DRenderTexture::Create(256.0f, 256.0f);
	//testshadow = DShader::Create("../Res/testshadow.shader");
	//m_mat = new DMaterial(testshadow);
}


TestFilter::~TestFilter()
{
}

void TestFilter::Render(DRenderTexture * screenTexture)
{
	m_glowMat->SetFloat("blackness", 0.2f);

	DGraphics::Blit(screenTexture, m_dst, m_glowMat);
	m_mat->SetVector2("offset", DVector2(0.01f, 0.0f));
	DGraphics::Blit(m_dst, m_dst2, m_mat);
	DGraphics::Blit(m_dst2, m_dst, m_mat);
	DGraphics::Blit(m_dst, m_dst2, m_mat);
	m_mat->SetVector2("offset", DVector2(0.00f, 0.01f));
	DGraphics::Blit(m_dst2, m_dst, m_mat);
	DGraphics::Blit(m_dst, m_dst2, m_mat);
	DGraphics::Blit(m_dst2, m_dst, m_mat);
	m_mixMat->SetTexture("mix", m_dst);
	DGraphics::Blit(screenTexture, m_mixMat);


	//DGraphics::Blit(screenTexture, m_mat);
}

void TestFilter::Release()
{
	if (m_dst != NULL)
	{
		m_dst->Destroy();
		delete m_dst;
		m_dst = NULL;
	}
	if (m_dst2 != NULL)
	{
		m_dst2->Destroy();
		delete m_dst2;
		m_dst2 = NULL;
	}
	/*if (m_mat != NULL)
	{
		m_mat->Destroy();
		delete m_mat;
		m_mat = NULL;
	}*/

}
