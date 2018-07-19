#pragma once
#include "DObject.h"
#include "DTransform.h"
#include "DCuller.h"
#include "DRender.h"

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
	bool CullObject(DCuller*, DRender*);
	DTransform* GetTransform() const;
	bool IsInitialized();
	bool IsDestroyed();

protected:
	virtual bool OnInit() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnFixedUpdate() = 0;
	virtual bool OnCullObject(DCuller*, DRender*) { return false; }

protected:
	DTransform* m_transform;

private:
	bool m_isInitialized;
	bool m_isDestroyed;

};

