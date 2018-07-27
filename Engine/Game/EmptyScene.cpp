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
	transform->SetEuler(38.503f, -285.023f, 0.0f);
	transform->SetPosition(4.298018f, 2.176152f, -3.152558f);

	DMaterial* colmat = DRes::Load<DMaterial>(DEFAULT_GROUP, COLOR_MAT);
	DGeometry* obj = DRes::Load<DGeometry>(DEFAULT_GROUP, BODY_MESH);
	//DGeometry* obj = DGeometry::Create(DGeometry_Plane);

	DDisplayObject* obj0 = new DDisplayObject(obj, colmat);
	obj0->Create();
	transform = obj0->GetTransform();
	transform->SetPosition(12.6f, -4.4f, -0.84f);
	transform->SetEuler(0.0f, -125.63f, 0.0f);

}

void EmptyScene::OnUnLoad()
{
	DRes::UnLoadGroup(DEFAULT_GROUP);
}
