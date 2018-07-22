#pragma once
#include "DScene.h"

/*框架入口基类*/
/*从该类扩展，并注册自己的场景*/
class DFrameBase
{
public:
	DFrameBase();
	~DFrameBase();
	void Create(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api);
	void Destroy();

protected:
	virtual void OnInit() = 0;

	/*注册场景*/
	void RegisterScene(DScene* scene);
	/*切换场景*/
	void ChangeScene(SCENEID sceneId);
};

