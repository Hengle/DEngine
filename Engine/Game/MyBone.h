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
public:
	MyBoneObj();
	~MyBoneObj();

public:
	void LoadBone(char*);

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

