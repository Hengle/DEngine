#pragma once
#include "DShader.h"

class DMaterial
{
public:
	DMaterial();
	~DMaterial();
	DShader* GetShader();
private:
	DShader* m_Shader;
	UINT m_stencilId;
	bool m_zwrite;

};

