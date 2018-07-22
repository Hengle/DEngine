#include "stdafx.h"
#include "EmptyScene.h"


EmptyScene::EmptyScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
}

void EmptyScene::OnLoad()
{
	DCamera* cam = new DCamera();
	cam->Create();
}
