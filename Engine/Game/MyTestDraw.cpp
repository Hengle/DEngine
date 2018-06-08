#include "stdafx.h"
#include "MyTestDraw.h"
#include "DSystem.h"
#include "D3D9Core.h"
#include "DModelLoader.h"

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
	
	//core->GetDevice()->CreateVertexBuffer(3 * sizeof(float)*7, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &m_mesh, 0);

	m_shader->Init(core->GetDevice(), L"../Res/color.vs9", L"../Res/color.ps9");


	float* vertices;
	unsigned long* indices;
	int vcount, icount, dsize, blen;
	DModelLoader::LoadObj("../Res/eboy.obj", &vertices, &indices, vcount, icount, blen, dsize);

	int fcount = icount / 3;

	D3DVERTEXELEMENT9 elemts[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};
	HRESULT res = D3DXCreateMesh(fcount, vcount, D3DXMESH_MANAGED, elemts, core->GetDevice(), &m_mesh);
	if (FAILED(res))
	{
		int u = 0;
	}

	int i;
	float* v;
	m_mesh->LockVertexBuffer(0, (void**)&v);

	for (i = 0; i < vcount*blen; i++)
	{
		v[i] = vertices[i];
	}

	/*v[0] = -1.0f;
	v[1] = 0.0f;
	v[2] = 2.0f;
	v[3] = 1.0f;
	v[4] = 0.0f;
	v[5] = 0.0f;
	v[6] = 1.0f;

	v[7] = 0.0f;
	v[8] = 1.0f;
	v[9] = 2.0f;
	v[10] = 0.0f;
	v[11] = 1.0f;
	v[12] = 0.0f;
	v[13] = 1.0f;

	v[14] = 1.0f;
	v[15] = 0.0f;
	v[16] = 2.0f;
	v[17] = 0.0f;
	v[18] = 0.0f;
	v[19] = 1.0f;
	v[20] = 1.0f;*/


	m_mesh->UnlockVertexBuffer();

	WORD*ind = 0;
	m_mesh->LockIndexBuffer(0, (void**)&ind);

	/*ind[0] = 0;
	ind[1] = 1;
	ind[2] = 2;*/
	for (i = 0; i < icount; i++)
	{
		ind[i] = indices[i];
	}

	m_mesh->UnlockIndexBuffer();

	/*DWORD* attributeBuffer = 0;
	m_mesh->LockAttributeBuffer(0, &attributeBuffer);

	attributeBuffer[0] = 0;

	m_mesh->UnlockAttributeBuffer();*/

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

	

	//Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	//Device->SetStreamSource(0, m_mesh, 0, sizeof(float)*7);

	// draw the triangle to the left with flat shading
	D3DXMatrixTranslation(&WorldMatrix, -1.25f, 0.0f, 0.0f);
	//Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

	//Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	//Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

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

	//m_mesh->DrawSubset(0);

	m_shader->SetWorldMatrix(core->GetDevice(), &WorldMatrix);

	m_shader->SetProjMatrix(core->GetDevice(), &proj);

	m_shader->Draw(core->GetDevice());

	m_mesh->DrawSubset(0);

	//Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

void MyTestDraw::Release()
{
	m_mesh->Release();
	m_mesh = 0;

	m_shader->Release();
	delete m_shader;
	m_shader = 0;
}
