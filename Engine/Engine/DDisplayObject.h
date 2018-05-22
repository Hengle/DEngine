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
	DDisplayObject(DMesh*, DLightShader*, DTexture*, DTexture*, DLight*);
	~DDisplayObject();
	virtual void Render();
	virtual void Init();
	virtual void Destroy();

protected:
	DMesh* m_mesh;
	DLightShader* m_shader;
	DTexture* m_texture;
	DTexture* m_decal;
	DLight* m_light;
};

