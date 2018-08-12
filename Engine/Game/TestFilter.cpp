#include "stdafx.h"
#include "TestFilter.h"
#include "DSystem.h"
#include "TestResDefine.h"

TestFilter::TestFilter()
{
	nearV = 0.02f;
	farV = 0.2f;
	//m_shader = DRes::Load<DShader>(DEFAULT_GROUP, PEFFECT_SHADER);
	//m_mat = new DMaterial(m_shader);
	//m_glowMat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3011);
	DShader* fov = DRes::Load<DShader>(DEFAULT_GROUP, 1014);
	//m_mixMat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3012);
	m_mixMat = new DMaterial(fov);
	m_mat = DRes::Load<DMaterial>(DEFAULT_GROUP, PEFFECT_MAT);
	//m_mat = DRes::Load<DMaterial>(DEFAULT_GROUP, 3013);
	//m_mat->SetFloat("offset", 0.008f);

	//m_dst = DRenderTexture::Create(64.0f, 64.0f);
	m_dst = DRenderTexture::Create(1024, 768);
	m_dst2 = DRenderTexture::Create(64.0f, 64.0f);
	//testshadow = DShader::Create("../Res/testshadow.shader");
	//m_mat = new DMaterial(testshadow);
}


TestFilter::~TestFilter()
{
}

DRenderTexture* TestFilter::Render(DRenderTexture * screenTexture)
{
	/*DGraphics::Blit(screenTexture, m_dst, m_mat);
	return m_dst;*/

	DGraphics::Blit(screenTexture, m_dst2);
	//m_glowMat->SetFloat("blackness", 0.2f);

	//DGraphics::Blit(m_dst, m_dst2, m_glowMat);
	m_mat->SetVector2("offset", DVector2(0.02f, 0.0f));
	DGraphics::Blit(m_dst2, m_dst, m_mat);
	DGraphics::Blit(m_dst, m_dst2, m_mat);
	DGraphics::Blit(m_dst2, m_dst, m_mat);
	m_mat->SetVector2("offset", DVector2(0.00f, 0.02f));
	DGraphics::Blit(m_dst, m_dst2, m_mat);
	DGraphics::Blit(m_dst2, m_dst, m_mat);
	DGraphics::Blit(m_dst, m_dst2, m_mat);

	m_mixMat->SetTexture("blurTex", m_dst2);
	m_mixMat->SetVector2("area", DVector2(nearV, farV));
	//m_mixMat->SetTexture("mix", m_dst2);
	DGraphics::Blit(screenTexture, m_mixMat);
	return NULL;
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
	if (m_mixMat != NULL)
	{
		m_mixMat->Destroy();
		delete m_mixMat;
		m_mixMat = NULL;
	}
	/*if (m_mat != NULL)
	{
		m_mat->Destroy();
		delete m_mat;
		m_mat = NULL;
	}*/

}
