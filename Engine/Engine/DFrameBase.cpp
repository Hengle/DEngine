#include "DFrameBase.h"
#include "DSystem.h"


DFrameBase::DFrameBase()
{
}


DFrameBase::~DFrameBase()
{
}

void DFrameBase::Create(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api)
{
	DSystem *system;
	bool result;
	system = DSystem::Create(api);
	if (!system)
		return;
	result = system->Init(screenWidth, screenHeight, fullScreen, api);

	if (result)
	{
		OnInit();
		system->Run();
	}
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
	DSystem::GetSceneMgr()->RegisterScene(scene);
}

void DFrameBase::ChangeScene(SCENEID sceneId)
{
	DSystem::GetSceneMgr()->ChangeScene(sceneId);
}
