#pragma once
#include "DShader.h"
#include "DSceneObject.h"
#include "DMesh.h"
#include "DTexture.h"
#include "DLight.h"

class DDisplayObject : public DSceneObject
{
public:
	DDisplayObject();
	DDisplayObject(DMesh*, DLightShader*, DTexture2D*, DLight*);
	~DDisplayObject();
	virtual void Render();
	virtual void Init();
	virtual void Destroy();

protected:
	DMesh* m_mesh;
	DLightShader* m_shader;
	DTexture2D* m_texture;
	//DTexture2D* m_decal;
	DLight* m_light;
};

