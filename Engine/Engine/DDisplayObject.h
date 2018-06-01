#pragma once
#include "DSceneObject.h"
#include "DMaterial.h"
#include "DMesh.h"
#include "DTexture.h"
#include "DLight.h"

class DDisplayObject : public DSceneObject
{
public:
	DDisplayObject(DMesh*, DMaterial*);
	~DDisplayObject();
	virtual void Render();
	virtual void Init();
	virtual void Destroy();

protected:
	DMesh* m_mesh;
	DMaterial* m_material;
};
