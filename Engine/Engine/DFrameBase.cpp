#include "DFrameBase.h"


DFrameBase::DFrameBase()
{
}


DFrameBase::~DFrameBase()
{
}

void DFrameBase::Create()
{
	DSystem *system;
	bool result;
	system = new DSystem;
	if (!system)
		return;
	result = system->Init();

	OnInit();

	if (result)
		system->Run();
	system->Shutdown();
	delete system;
}

void DFrameBase::Destroy()
{
}

void DFrameBase::RegisterScene(DScene * scene)
{
	if (scene == NULL)
		return;
	DSystem::GetSceneManager()->RegisterScene(scene);
}

void DFrameBase::ChangeScene(SCENEID sceneId)
{
	DSystem::GetSceneManager()->ChangeScene(sceneId);
}
