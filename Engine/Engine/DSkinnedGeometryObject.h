#pragma once
#include "DDisplayObject.h"

class DSkinnedGeometryObject : public DDisplayObject
{
public:
	DSkinnedGeometryObject(DGeometry*, DMaterial*, DTransform** bones, int boneCount);
	~DSkinnedGeometryObject();

	int GetBoneCount() const;
	DTransform* GetBone(int index) const;

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnFixedUpdate();
	virtual bool OnCullObject();
	virtual void OnRenderObject();

private:
	DTransform** m_bones;
	int m_boneCount;
};

