#pragma once
#include "DObject.h"
#include "DTransform.h"

#define DLAYER unsigned int

#define DLAYER_DEFAULT 1U

class DTransform;

/*场景对象*/
class DSceneObject : public DObject
{
public:
	DSceneObject();
	~DSceneObject();
	void Create();
	void Destroy();
	void Update();
	void FixedUpdate();
	bool CullObject();
	void RenderObject();
	DTransform* GetTransform() const;
	bool IsInitialized();
	bool IsDestroyed();
	void SetLayer(DLAYER);
	DLAYER GetLayer();
	

protected:
	virtual bool OnInit() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnFixedUpdate() = 0;
	virtual bool OnCullObject() { return false; }
	virtual void OnRenderObject() {}

protected:
	DTransform* m_transform;
	DLAYER m_layer;

private:
	bool m_isInitialized;
	bool m_isDestroyed;

};

