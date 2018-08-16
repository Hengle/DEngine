#include "DSkinnedGeometryObject.h"


DSkinnedGeometryObject::DSkinnedGeometryObject(DGeometry * geometry, DMaterial * material, DTransform ** bones, int boneCount) : DDisplayObject(geometry, material)
{
	m_bones = bones;
	m_boneCount = boneCount;
}

DSkinnedGeometryObject::~DSkinnedGeometryObject()
{
}

int DSkinnedGeometryObject::GetBoneCount() const
{
	return m_boneCount;
}

DTransform * DSkinnedGeometryObject::GetBone(int index) const
{
	if (index < 0 || index >= m_boneCount)
		return NULL;
	return m_bones[index];
}

void DSkinnedGeometryObject::OnDestroy()
{
	DDisplayObject::OnDestroy();
	delete[] m_bones;
	m_bones = 0;
}

bool DSkinnedGeometryObject::OnCullObject()
{
	if (!DDisplayObject::OnCullObject())
		return false;



	return true;
}
