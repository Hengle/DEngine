﻿#pragma once
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
	DCamera* m_camera;
	//DMesh* cube;
	//DMesh* obj;
	DRenderTexture* m_rt;
	DLight* m_light;
	DVector3 m_lookAtPoint;
	float m_lookDistance;
	/*DMesh* testcolormesh;
	DDisplayObject* testcolorobj;*/
	
};