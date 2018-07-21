#pragma once
#include "DSceneObject.h"
#include "DMaterial.h"
#include "DGeometry.h"
#include "DLight.h"

class DDisplayObject;

struct DDisplayObjectNode
{
public:
	DDisplayObject* object;
	DDisplayObjectNode* next;
	DDisplayObjectNode* pre;
};

/*显示对象*/
class DDisplayObject : public DSceneObject
{
public:
	DDisplayObject(DGeometry*, DMaterial*);
	~DDisplayObject();
	//virtual void Render(DShader* replaceShader);
	/*设置可见性*/
	void SetVisible(bool visible /*是否可见*/);
	/*获取可见性*/
	bool GetVisible();
	DMaterial* GetMaterial();
	DGeometry* GetGeometry();

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual bool OnCullObject(DCuller*);
	virtual void OnRenderObject();

protected:
	DGeometry* m_geometry;
	DMaterial* m_material;

private:
	bool m_isVisible;
};
