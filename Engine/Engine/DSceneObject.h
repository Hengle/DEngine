#pragma once
#include "DObject.h"
#include "DTransform.h"

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
	void Render();
	void Cull();
	DTransform* GetTransform() const;
	bool IsInitialized();

protected:
	virtual bool OnInit() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnFixedUpdate() = 0;
	virtual void OnRender() {}
	virtual void OnCull() {}

protected:
	DTransform* m_transform;

private:
	bool m_isInitialized;

};

