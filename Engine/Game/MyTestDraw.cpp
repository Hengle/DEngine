#include "stdafx.h"
#include "MyTestDraw.h"
#include "DSystem.h"
#include "D3D9Core.h"

//struct ColorVertex
//{
//	ColorVertex() {}
//
//	ColorVertex(float x, float y, float z, D3DCOLOR c)
//	{
//		_x = x;	 _y = y;  _z = z;  _color = c;
//	}
//
//	float _x, _y, _z;
//	D3DCOLOR _color;
//
//	static const DWORD FVF2;
//};
//const DWORD ColorVertex::FVF2 = D3DFVF_XYZ | D3DFVF_DIFFUSE;

MyTestDraw::MyTestDraw()
{
	m_mesh = 0;
	m_shader = 0;
}


MyTestDraw::~MyTestDraw()
{
}

void MyTestDraw::Init()
{
	m_shader = new MyTestShader();
	
	
	D3D9Core* core = (D3D9Core*)DSystem::GetGraphicsMgr()->GetGLCore();
	core->GetDevice()->CreateVertexBuffer(3 * sizeof(float)*7, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_XYZW, D3DPOOL_MANAGED, &m_mesh, 0);

	m_shader->Init(core->GetDevice(), L"../Res/color.vs9", L"../Res/color.ps9");

	float* v;
	m_mesh->Lock(0, 0, (void**)&v, 0);

	/*v[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = ColorVertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	v[2] = ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));*/

	v[0] = -1.0f;
	v[1] = 0.0f;
	v[2] = 2.0f;
	v[3] = 255.0f;
	v[4] = 255.0f;
	v[5] = 0.0f;
	v[6] = 0.0f;

	v[7] = 0.0f;
	v[8] = 1.0f;
	v[9] = 2.0f;
	v[10] = 1.0f;
	v[11] = 0.0f;
	v[12] = 1.0f;
	v[13] = 0.0f;

	v[14] = 1.0f;
	v[15] = 0.0f;
	v[16] = 2.0f;
	v[17] = 1.0f;
	v[18] = 0.0f;
	v[19] = 0.0f;
	v[20] = 1.0f;


	m_mesh->Unlock();

	//
	// Turn off lighting.
	//

	//core->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
}

void MyTestDraw::Render()
{
	D3D9Core* core = (D3D9Core*)DSystem::GetGraphicsMgr()->GetGLCore();
	float w, h;
	DSystem::GetGraphicsMgr()->GetResolution(w, h);

	D3DXMATRIX WorldMatrix;

	LPDIRECT3DDEVICE9 Device = core->GetDevice();

	Device->SetFVF(D3DFVF_XYZ | D3DFVF_XYZW);
	Device->SetStreamSource(0, m_mesh, 0, sizeof(float)*7);

	// draw the triangle to the left with flat shading
	D3DXMatrixTranslation(&WorldMatrix, -1.25f, 0.0f, 0.0f);
	//Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// draw the triangle to the right with gouraud shading
	//D3DXMatrixTranslation(&WorldMatrix, 1.25f, 0.0f, 0.0f);
	//Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)w / (float)h,
		1.0f,
		1000.0f);
	//core->GetDevice()->SetTransform(D3DTS_PROJECTION, &proj);

	m_shader->SetWorldMatrix(core->GetDevice(), &WorldMatrix);

	m_shader->SetProjMatrix(core->GetDevice(), &proj);

	m_shader->Draw(core->GetDevice());

	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

void MyTestDraw::Release()
{
	m_mesh->Release();
	m_mesh = 0;

	m_shader->Release();
	delete m_shader;
	m_shader = 0;
}
