#include "stdafx.h"
#include "MyBone.h"
#include "DRes.h"
#include "DGraphics.h"
#include "DLog.h"
#include <fstream>

MyBone::MyBone() : DSceneObject()
{
	m_material = 0;
}


MyBone::~MyBone()
{
}

bool MyBone::OnInit()
{
	DShader* shader = DRes::LoadInternal<DShader>(D_RES_SHADER_COLOR);
	m_material = new DMaterial(shader);
	return true;
}

void MyBone::OnDestroy()
{
	if (m_material != NULL)
	{
		m_material->Destroy();
		delete m_material;
		m_material = 0;
	}
}

void MyBone::OnUpdate()
{
}

void MyBone::OnFixedUpdate()
{
}

bool MyBone::OnCullObject()
{
	m_material->SetPass(0);
	DGraphics::GlPushMatrix();

	DGraphics::GlBegin(DGeometryTopology_LineList);

	DGraphics::GlVertex3(0.0f, 0.0f, 0.0f);
	DGraphics::GlVertex3(-0.342f, 0.0f, -0.814f);

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();
	return true;
}

struct BoneData
{
public:
	MyBone* bone;
	int parent;
	float posx, posy, posz, rotx, roty, rotz, rotw;
};

MyBoneObj::MyBoneObj()
{
}

MyBoneObj::~MyBoneObj()
{
}

void MyBoneObj::LoadBone(char * path)
{
	std::ifstream ifile;
	ifile.open(path);
	if (ifile.fail())
	{
		return;
	}

	char boneName[64];
	float posx, posy, posz, rotx, roty, rotz, rotw;
	int index;

	while (!ifile.eof())
	{
		ifile >> boneName >> posx >> posy >> posz >> rotx >> roty >> rotz >> rotw >> index;
		
		DLog::InfoArgs("BoneName:%s,Pos:(%f,%f,%f),Rot:(%f,%f,%f,%f),index:%d", boneName, posx, posy, posz, rotx, roty, rotz, rotw, index);

		MyBone* bone = new MyBone();
		bone->Create();
		m_bones.push_back(bone);
	}

	ifile.close();
}

bool MyBoneObj::OnInit()
{
	return true;
}

void MyBoneObj::OnDestroy()
{
}

void MyBoneObj::OnUpdate()
{
}

void MyBoneObj::OnFixedUpdate()
{
}

bool MyBoneObj::OnCullObject()
{
	return true;
}
