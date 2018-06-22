#pragma once
#include "DScene.h"
#include "DDisplayObject.h"
#include "DShader.h"
#include "DMesh.h"
#include "MyTestDraw.h"

class TestScene : public DScene
{
public:
	TestScene(SCENEID sceneId, char* sceneName);

protected:
	virtual void OnGUI();
	virtual void OnLoad();
	virtual void OnUnLoad();
	virtual void OnRender();
	virtual void OnUpdate();

private:
	void TestLoad();

private:
	//ModelClass* m_Model;
	//ColorShaderClass* m_ColorShader;
	DDisplayObject* m_plane;
	DDisplayObject* m_obj0;
	MyTestDraw *testd;
	DMesh* plane;
	DShader* shader;
	DMaterial* mat;
	DTexture* floor;
	DTexture* map;
};