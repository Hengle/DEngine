#pragma once
#include "DScene.h"
#include "DDisplayObject.h"
#include "DShader.h"
#include "DMesh.h"
#include "TestFilter.h"

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
	TestFilter* m_filter;
	DDisplayObject* m_plane;
	DDisplayObject* m_obj0;
	DDisplayObject* m_cube;
	DMesh* plane;
	DMesh* cube;
	DMesh* obj;
	DShader* shader;
	DMaterial* mat;
	DMaterial* mat2;
	DMaterial* mat3;
	DTexture* floor;
	DTexture* map;
	DTexture* cb;
	DRenderTexture* m_rt;


	DShader* testcolorshader;
	DMaterial* testcolormat;
	DMesh* testcolormesh;
	DDisplayObject* testcolorobj;
};