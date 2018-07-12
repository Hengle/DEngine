#pragma once
#include "DScene.h"

/*�����ڻ���*/
/*�Ӹ�����չ����ע���Լ��ĳ���*/
class DFrameBase
{
public:
	DFrameBase();
	~DFrameBase();
	void Create();
	void Destroy();

protected:
	virtual void OnInit() = 0;

	/*ע�᳡��*/
	void RegisterScene(DScene* scene);
	/*�л�����*/
	void ChangeScene(SCENEID sceneId);
};

