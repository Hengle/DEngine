#pragma once
#include "DSceneObject.h"
#include "DMaterial.h"
#include <vector>
#include <fstream>

class MyBone : public DSceneObject
{
public:
	MyBone();
	~MyBone();

	void SetBoneName(char*);
	const char* GetBoneName();

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual bool OnCullObject();

private:
	char* m_boneName;
};

class MyBoneObj : public DSceneObject
{
private:
	class PoseKey
	{
	public:
		void Release();
		virtual void Sample(DTransform* transform, float time) {}
		void Reset() { m_currentKey = 0; }

	protected:
		std::vector<float*> m_keys;
		int m_currentKey = 0;
	};
	class PositionKey : public PoseKey
	{
	public:
		void Insert(float x, float y, float z, float time);
		virtual void Sample(DTransform* transform, float time);
	};
	class RotationKey : public PoseKey
	{
	public:
		void Insert(float x, float y, float z, float w, float time);
		virtual void Sample(DTransform* transform, float time);
	};
	class ScaleKey : public PoseKey
	{
	public:
		void Insert(float x, float y, float z, float time);
		virtual void Sample(DTransform* transform, float time);
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

		void Sample(DTransform* transform, float time);
		void Reset();
		void Release();
	};

public:
	MyBoneObj();
	~MyBoneObj();

public:
	void LoadBone(char*);
	void LoadAnim(char*);
	int GetBoneCount() const;
	MyBone* GetBone(int index);
	void GetBoneMatrix(int index, DMatrix4x4&);
	DTransform** GetBones();
	void Play();
	void Stop();

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual bool OnCullObject();

private:
	void LoadAnimCurves(std::ifstream&, MyBoneData*);
	void LoadPositionKeys(std::ifstream&, MyBoneData*, int);
	void LoadRotationKeys(std::ifstream&, MyBoneData*, int);
	void LoadScaleKeys(std::ifstream&, MyBoneData*, int);

private:
	std::vector<MyBone*> m_bones;
	std::vector<MyBoneData> m_boneDatas;
	DTransform** m_realBones;
	DMaterial* m_material;
	float m_maxTime;
	float m_currentTime;

	bool m_isPlaying;
};

