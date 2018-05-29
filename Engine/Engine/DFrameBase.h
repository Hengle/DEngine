#pragma once
#include "DScene.h"

class DFrameBase
{
public:
	DFrameBase();
	~DFrameBase();
	void Create();
	void Destroy();

protected:
	virtual void OnInit() = 0;

	void RegisterScene(DScene* scene);
	void ChangeScene(SCENEID sceneId);
};

