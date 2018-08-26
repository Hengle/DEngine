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
	DTransform* parent = m_transform->GetParent();
	if (parent == NULL)
		return false;

	m_material->SetPass(0);
	DGraphics::GlPushMatrix();

	DGraphics::GlBegin(DGeometryTopology_LineList);

	DVector3 pos, parentpos;
	parent->GetPosition(pos);
	m_transform->GetPosition(parentpos);


	DGraphics::GlVertex(pos);
	DGraphics::GlVertex(parentpos);

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

	vector<BoneData> boneDatas;

	while (!ifile.eof())
	{
		ifile >> boneName >> posx >> posy >> posz >> rotx >> roty >> rotz >> rotw >> index;
		
		DLog::InfoArgs("BoneName:%s,Pos:(%f,%f,%f),Rot:(%f,%f,%f,%f),index:%d", boneName, posx, posy, posz, rotx, roty, rotz, rotw, index);

		BoneData data;
		data.bone = new MyBone();
		data.bone->Create();
		data.parent = index;
		data.posx = posx;
		data.posy = posy;
		data.posz = posz;
		data.rotx = rotx;
		data.roty = roty;
		data.rotz = rotz;
		data.rotw = rotw;

		boneDatas.push_back(data);
	}

	ifile.close();

	int i;
	for (i = 0; i < boneDatas.size(); i++)
	{
		BoneData data = boneDatas[i];
		DTransform* transform = data.bone->GetTransform();
		if (data.parent == -1)
		{
			transform->SetParent(m_transform);
		}
		else
		{
			DTransform* parent = boneDatas[data.parent].bone->GetTransform();
			transform->SetParent(parent);
		}
		transform->SetLocalPosition(data.posx, data.posy, data.posz);
		transform->SetLocalRotation(data.rotx, data.roty, data.rotz, data.rotw);
		m_bones.push_back(data.bone);
	}
}

bool MyBoneObj::OnInit()
{
	return true;
}

void MyBoneObj::OnDestroy()
{
	/*int i;
	for (i = 0; i < m_bones.size(); i++)
	{
		MyBone* bone = m_bones[i];
		bone->Destroy();
		delete bone;
		bone = NULL;
	}*/
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
