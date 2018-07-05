#pragma once
#include "DSceneObject.h"
#include "DMaterial.h"
#include "DMesh.h"
#include "DLight.h"

/*显示对象*/
class DDisplayObject : public DSceneObject
{
public:
	DDisplayObject(DMesh*, DMaterial*);
	~DDisplayObject();
	virtual void Render(DShader* replaceShader);
	virtual void Init();
	virtual void Destroy();
	/*设置可见性*/
	void SetVisible(bool visible /*是否可见*/);
	/*获取可见性*/
	bool GetVisible();

protected:
	DMesh* m_mesh;
	DMaterial* m_material;

private:
	bool m_isVisible;
};
