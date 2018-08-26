#include "stdafx.h"
#include "MyBone.h"
#include "DRes.h"
#include "DGraphics.h"
#include "DLog.h"
#include <fstream>

MyBone::MyBone() : DSceneObject()
{
	
}


MyBone::~MyBone()
{
}

bool MyBone::OnInit()
{
	
	return true;
}

void MyBone::OnDestroy()
{
	
}

void MyBone::OnUpdate()
{
}

void MyBone::OnFixedUpdate()
{
}

bool MyBone::OnCullObject()
{
	
	return true;
}

struct BoneData
{
public:
	MyBone* bone;
	int parent;
	float posx, posy, posz, rotx, roty, rotz, rotw;
};

MyBoneObj::MyBoneObj() : DSceneObject()
{
	m_material = 0;
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
		//data.bone->Create();
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

void MyBoneObj::LoadAnim(char * path)
{
	std::ifstream ifile;
	ifile.open(path);
	if (ifile.fail())
	{
		return;
	}
	int boneIndex;
	while (!ifile.eof())
	{
		ifile >> boneIndex;
	}

	ifile.close();
}

bool MyBoneObj::OnInit()
{
	DShader* shader = DRes::LoadInternal<DShader>(D_RES_SHADER_COLOR);
	m_material = new DMaterial(shader);
	return true;
}

void MyBoneObj::OnDestroy()
{
	if (m_material != NULL)
	{
		m_material->Destroy();
		delete m_material;
		m_material = 0;
	}
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
	if (m_bones.size() == 0)
		return false;

	m_material->SetPass(0);
	DGraphics::GlPushMatrix();

	DGraphics::GlBegin(DGeometryTopology_LineList);

	int i;
	for (i = 0; i < m_bones.size(); i++)
	{
		DVector3 pos, parentpos;
		DTransform* chi = m_bones[i]->GetTransform();
		chi->GetPosition(pos);
		chi->GetParent()->GetPosition(parentpos);

		DGraphics::GlVertex(pos);
		DGraphics::GlVertex(parentpos);
	}

	DGraphics::GlEnd();

	DGraphics::GlPopMatrix();
	return true;
}
