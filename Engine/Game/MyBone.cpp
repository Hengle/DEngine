#include "stdafx.h"
#include "MyBone.h"
#include "DRes.h"
#include "DGraphics.h"
#include "DLog.h"
#include "DTime.h"
#include "TestResDefine.h"

MyBone::MyBone() : DSceneObject()
{
	m_boneName = 0;
}


MyBone::~MyBone()
{
}

void MyBone::SetBoneName(char * boneName)
{
	int len = strlen(boneName) + 1;
	m_boneName = new char[len];
	strcpy_s(m_boneName, len, boneName);
}

const char * MyBone::GetBoneName()
{
	return m_boneName;
}

bool MyBone::OnInit()
{
	
	return true;
}

void MyBone::OnDestroy()
{
	if (m_boneName != 0)
		delete[] m_boneName;
	m_boneName = 0;
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
	m_maxTime = 0;
	m_currentTime = 0;
	m_isPlaying = false;
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
		data.bone->SetBoneName(boneName);

		boneDatas.push_back(data);
	}

	ifile.close();

	int i;

	DGeometry* cube = DRes::LoadInternal<DGeometry>(D_RES_MESH_CUBE);
	DMaterial* mat3 = DRes::Load<DMaterial>(DEFAULT_GROUP, DECAL_MAT);
	DDisplayObject* cubeobj;
	DTransform* tr;
	

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

		cubeobj = new DDisplayObject(cube, mat3);
		cubeobj->Create();
		tr = cubeobj->GetTransform();
		tr->SetLocalScale(0.02f, 0.02f, 0.02f);
		tr->SetParent(transform);
		tr->SetLocalPosition(0, 0, 0);

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
	char read[32];
	MyBoneData bonedata;
	
	//bool isBegin = false;
	while (!ifile.eof())
	{
		ifile >> read;

		//if (!isBegin)
		{
			if (strcmp(read, "bonebegin") == 0)
			{
				ifile >> boneIndex;
				bonedata = MyBoneData();
				bonedata.boneIndex = boneIndex;
				bonedata.positionKey = 0;
				bonedata.rotationKey = 0;
				bonedata.scaleKey = 0;
				//isBegin = true;

				LoadAnimCurves(ifile, &bonedata);

				m_boneDatas.push_back(bonedata);
			}
		}
		//else
		/*{
			if (strcmp(read, "boneend") == 0)
			{
				isBegin = false;
				m_boneDatas.push_back(bonedata);
			}
		}*/
		
	}

	ifile.close();
}

int MyBoneObj::GetBoneCount() const
{
	return m_bones.size();
}

MyBone * MyBoneObj::GetBone(int index)
{
	if (index >= 0 && index < m_bones.size())
	{
		return m_bones[index];
	}
	return nullptr;
}

void MyBoneObj::Play()
{
	m_isPlaying = true;
}

void MyBoneObj::Stop()
{
	m_isPlaying = false;
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
	int i;
	for (i = 0; i < m_boneDatas.size(); i++)
	{
		MyBoneData bonedata = m_boneDatas[i];
		bonedata.Release();
	}
}

void MyBoneObj::OnUpdate()
{
	if (!m_isPlaying)
		return;
	bool reset = false;
	
	if (m_currentTime > m_maxTime) {
		m_currentTime = 0;
		reset = true;
	}

	int i;
	for (i = 0; i < m_boneDatas.size(); i++)
	{
		MyBoneData bonedata = m_boneDatas[i];
		if (reset)
			bonedata.Reset();
		if (bonedata.boneIndex >= 0 && bonedata.boneIndex < m_bones.size()) {
			MyBone* bone = m_bones[bonedata.boneIndex];
			DTransform* transform = bone->GetTransform();
			bonedata.Sample(transform, m_currentTime);
		}
	}

	m_currentTime += DTime::GetDeltaTime();
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

void MyBoneObj::LoadPositionKeys(std::ifstream & ifile, MyBoneData *bonedata, int count)
{
	int i = 0;
	float x, y, z, time;
	for (i = 0; i < count; i++)
	{
		ifile >> x >> y >> z>>time;
		if (time > m_maxTime)
			m_maxTime = time;
		if (bonedata->positionKey == 0)
			bonedata->positionKey = new PositionKey();
		bonedata->positionKey->Insert(x, y, z, time);
	}
}

void MyBoneObj::LoadRotationKeys(std::ifstream & ifile, MyBoneData *bonedata, int count)
{
	int i = 0;
	float x, y, z, w, time;
	for (i = 0; i < count; i++)
	{
		ifile >> x >> y >> z >> w >> time;
		if (time > m_maxTime)
			m_maxTime = time;
		if (bonedata->rotationKey == 0)
			bonedata->rotationKey = new RotationKey();
		bonedata->rotationKey->Insert(x, y, z, w, time);
	}
}

void MyBoneObj::LoadScaleKeys(std::ifstream & ifile, MyBoneData *bonedata, int count)
{
	int i = 0;
	float x, y, z, time;
	for (i = 0; i < count; i++)
	{
		ifile >> x >> y >> z >> time;
		if (time > m_maxTime)
			m_maxTime = time;
		if (bonedata->scaleKey == 0)
			bonedata->scaleKey = new ScaleKey();
		bonedata->scaleKey->Insert(x, y, z, time);
	}
}

void MyBoneObj::LoadAnimCurves(std::ifstream & ifile, MyBoneData * bonedata)
{
	int keycount;
	char read[32];
	while (!ifile.eof())
	{
		ifile >> read;

		if (strcmp(read, "p") == 0)
		{
			ifile >> keycount;
			LoadPositionKeys(ifile, bonedata, keycount);
		}
		else if (strcmp(read, "r") == 0)
		{
			ifile >> keycount;
			LoadRotationKeys(ifile, bonedata, keycount);
		}
		else if (strcmp(read, "s") == 0)
		{
			ifile >> keycount;
			LoadScaleKeys(ifile, bonedata, keycount);
		}
		else if (strcmp(read, "boneend") == 0)
		{
			return;
		}
	}
}

void MyBoneObj::PoseKey::Release()
{
	int i;
	for (i = 0; i < m_keys.size(); i++)
	{
		delete[] m_keys[i];
	}
	m_keys.clear();
}

void MyBoneObj::MyBoneData::Sample(DTransform * transform, float time)
{
	if (positionKey != 0)
		positionKey->Sample(transform, time);
	if (rotationKey != 0)
		rotationKey->Sample(transform, time);
	if (scaleKey != 0)
		scaleKey->Sample(transform, time);
}

void MyBoneObj::MyBoneData::Reset()
{
	if (positionKey != 0)
		positionKey->Reset();
	if (rotationKey != 0)
		rotationKey->Reset();
	if (scaleKey != 0)
		scaleKey->Reset();
}

void MyBoneObj::MyBoneData::Release()
{
	if (positionKey != 0)
	{
		positionKey->Release();
		delete positionKey;
		positionKey = 0;
	}
	if (rotationKey != 0)
	{
		rotationKey->Release();
		delete rotationKey;
		rotationKey = 0;
	}
	if (scaleKey != 0)
	{
		scaleKey->Release();
		delete scaleKey;
		scaleKey = 0;
	}
}

void MyBoneObj::ScaleKey::Insert(float x, float y, float z, float time)
{
	float* keys = new float[4];
	keys[0] = x;
	keys[1] = y;
	keys[2] = z;
	keys[3] = time;
	m_keys.push_back(keys);
}

void MyBoneObj::ScaleKey::Sample(DTransform * transform, float time)
{
	if (m_keys.size() < 2)
		return;
	if (m_currentKey == m_keys.size() - 1)
		return;

	float* begin = m_keys[m_currentKey];
	float* end = m_keys[m_currentKey + 1];

	float lp = (time - begin[3]) / (end[3] - begin[3]);
	if (lp < 0.0f)
		lp = 0.0f;
	if (lp > 1.0f)
		lp = 1.0f;

	DVector3 scale;
	transform->GetLocalScale(scale);

	DVector3::Lerp(DVector3(begin[0], begin[1], begin[2]), DVector3(end[0], end[1], end[2]), lp, scale);

	transform->SetLocalScale(scale);

	if (time >= end[3]) {
		m_currentKey += 1;
	}
}

void MyBoneObj::RotationKey::Insert(float x, float y, float z, float w, float time)
{
	float* keys = new float[5];
	keys[0] = x;
	keys[1] = y;
	keys[2] = z;
	keys[3] = w;
	keys[4] = time;

	//DQuaternion rot = DQuaternion(x, y, z, w);
	//DVector3 euler;
	//rot.EulerAngle(euler);
	//float* keys = new float[4];
	//keys[0] = euler.x;
	//keys[1] = euler.y;
	//keys[2] = euler.z;
	////keys[3] = w;
	//keys[3] = time;
	m_keys.push_back(keys);
}

void MyBoneObj::RotationKey::Sample(DTransform * transform, float time)
{
	if (m_keys.size() < 2)
		return;
	if (m_currentKey == m_keys.size() - 1)
		return;

	float* begin = m_keys[m_currentKey];
	float* end = m_keys[m_currentKey + 1];

	float lp = (time - begin[4]) / (end[4] - begin[4]);
	if (lp < 0.0f)
		lp = 0.0f;
	if (lp > 1.0f)
		lp = 1.0f;

	DQuaternion rotation;
	transform->GetLocalRotation(rotation);

	DQuaternion::Slerp(DQuaternion(begin[0], begin[1], begin[2], begin[3]), DQuaternion(end[0], end[1], end[2], end[3]), lp, rotation);

	transform->SetLocalRotation(rotation);

	/*DVector3 rotation;
	transform->GetLocalEuler(rotation);

	DVector3::Lerp(DVector3(begin[0], begin[1], begin[2]), DVector3(end[0], end[1], end[2]), lp, rotation);

	transform->SetLocalEuler(rotation);*/

	if (time >= end[4]) {
		m_currentKey += 1;
	}
}

void MyBoneObj::PositionKey::Insert(float x, float y, float z, float time)
{
	float* keys = new float[4];
	keys[0] = x;
	keys[1] = y;
	keys[2] = z;
	keys[3] = time;
	m_keys.push_back(keys);
}

void MyBoneObj::PositionKey::Sample(DTransform * transform, float time)
{
	if (m_keys.size() < 2)
		return;
	if (m_currentKey == m_keys.size() - 1)
		return;

	float* begin = m_keys[m_currentKey];
	float* end = m_keys[m_currentKey + 1];

	float lp = (time - begin[3]) / (end[3] - begin[3]);
	if (lp < 0.0f)
		lp = 0.0f;
	if (lp > 1.0f)
		lp = 1.0f;

	DVector3 position;
	transform->GetLocalPosition(position);

	DVector3::Lerp(DVector3(begin[0], begin[1], begin[2]), DVector3(end[0], end[1], end[2]), lp, position);

	transform->SetLocalPosition(position);

	if (time >= end[3]) {
		m_currentKey += 1;
	}
}
