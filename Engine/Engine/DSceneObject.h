#pragma once
#include "DObject.h"
#include "DTransform.h"

class DSceneObject : public DObject
{
public:
	DSceneObject();
	~DSceneObject();
	virtual void Init();
	virtual void Destroy();
	virtual void Update();
	virtual void FixedUpdate();
	DTransform* GetTransform() const;

protected:
	DTransform* m_Transform;

private:
	bool m_isPrepareDestroy;
};

