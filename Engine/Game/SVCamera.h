#pragma once
#include "DCamera.h"

class SVCamera : public DCamera
{
public:
	SVCamera();
	~SVCamera();

protected:
	virtual bool OnInit();
	virtual void OnDestroy();
	virtual void OnPostRender();

private:
	DMaterial* m_lineMat;
};

