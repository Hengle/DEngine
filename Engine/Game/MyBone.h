#pragma once
#include "DSceneObject.h"
#include "DMaterial.h"
#include <vector>

class MyBone : public DSceneObject
{
public:
	MyBone();
	~MyBone();

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual bool OnCullObject();

};

class MyBoneObj : public DSceneObject
{
private:
	class PoseKey
	{
	protected:
		std::vector<float*> m_keys;
	};
	class PositionKey : public PoseKey
	{
	public:
		
	};
	class RotationKey : public PoseKey
	{

	};
	class ScaleKey : public PoseKey
	{

	};
	class MyBoneData
	{
	public:
		int boneIndex;
		PositionKey* positionKey;
		RotationKey* rotationKey;
		ScaleKey* scaleKey;

		MyBoneData()
		{
			boneIndex = -1;
			positionKey = 0;
			rotationKey = 0;
			scaleKey = 0;
		}
	};

public:
	MyBoneObj();
	~MyBoneObj();

public:
	void LoadBone(char*);
	void LoadAnim(char*);

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual bool OnCullObject();

private:
	std::vector<MyBone*> m_bones;
	DMaterial* m_material;
};

