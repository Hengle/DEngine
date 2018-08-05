#include "stdafx.h"
#include "SVCamera.h"
#include "DRes.h"
#include "TestResDefine.h"
#include "DGraphics.h"

SVCamera::SVCamera() : DCamera()
{
	m_sortOrder = 1;
	m_layerMask = D_LAYERMASK_NOTHING;
	m_clearFlags = DClearFlags_Depth;
}


SVCamera::~SVCamera()
{
}

bool SVCamera::OnInit()
{
	if (DCamera::OnInit())
	{
		m_lineMat = DRes::Load<DMaterial>(DEFAULT_GROUP, COLOR_MAT);
		//SetOrthographic(true);
		SetBackgroundColor(DColor(0, 0, 0, 0));
		return true;
	}
	return false;
}

void SVCamera::OnDestroy()
{
	DCamera::OnDestroy();
	m_lineMat = NULL;
}

void SVCamera::OnPostRender()
{
	DCamera::OnPostRender();

	/*m_lineMat->SetPass(0);

	DGraphics::GlPushMatrix();
	DGraphics::GlLoadOrtho();
	DGraphics::GlBegin();

	DGraphics::GlVertex3(0, 0, 0.0f);
	DGraphics::GlVertex3(3.0f, 3.0f, 3.0f);

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();*/
}
