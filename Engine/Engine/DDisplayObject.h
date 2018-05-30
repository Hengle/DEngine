#pragma once
#include "DSceneObject.h"
#include "DShader.h"
#include "DMesh.h"
#include "DTexture.h"
#include "DLight.h"

class DDisplayObject : public DSceneObject
{
public:
	DDisplayObject(DMesh*, DShader*);
	~DDisplayObject();
	virtual void Render();
	virtual void Init();
	virtual void Destroy();

protected:
	DMesh* m_mesh;
	DShader* m_shader;
};
