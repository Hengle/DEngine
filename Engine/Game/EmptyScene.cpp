#include "stdafx.h"
#include "EmptyScene.h"
#include "DRes.h"
#include "TestResDefine.h"

EmptyScene::EmptyScene(SCENEID sceneId, char * sceneName) : DScene(sceneId, sceneName)
{
}

void EmptyScene::OnLoad()
{
	DCamera* cam = new DCamera();
	cam->Create();

	DTransform* transform;

	transform = cam->GetTransform();
	transform->SetEuler(34.996f, -154.423f, 0.0f);
	transform->SetPosition(3.24f, 6.822f, 7.701f);

	DMaterial* colmat = DRes::Load<DMaterial>(DEFAULT_GROUP, COLOR_MAT);
	DGeometry* obj = DRes::Load<DGeometry>(DEFAULT_GROUP, BODY_MESH);

	DDisplayObject* obj0 = new DDisplayObject(obj, colmat);
	obj0->Create();
	transform = obj0->GetTransform();
	transform->SetPosition(0.0f, 1.64f, 0.0f);
	transform->SetEuler(0.0f, 65.979f, 0.0f);

}

void EmptyScene::OnUnLoad()
{
	DRes::UnLoadGroup(DEFAULT_GROUP);
}
