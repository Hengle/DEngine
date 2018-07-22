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
	void GetBounds(DBounds*);

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual bool OnCullObject();
	virtual void OnRenderObject();

private:
	void UpdateBounds();

protected:
	DGeometry* m_geometry;
	DMaterial* m_material;
	DMaterial* m_gizmoMat;
	DBounds m_bounds;

private:
	bool m_isVisible;
};
