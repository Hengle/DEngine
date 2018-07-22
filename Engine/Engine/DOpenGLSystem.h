#pragma once
#ifdef _DGAPI_OPENGL

#include "DSystem.h"

class DOpenGLSystem : public DSystem
{
public:
	DOpenGLSystem();
	~DOpenGLSystem();

protected:
	virtual bool OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api);
	virtual void OnShutdown();
};
#endif
