#pragma once
#include "DShader.h"
#include <map>

class DMaterial : public DResObject
{
public:
	DMaterial(DShader*);
	~DMaterial();
	DShader* GetShader() const;
	void SetMatrix(const LPCSTR key, DMatrix4x4&);
	void SetVector4(const LPCSTR key, DVector4&);
	void SetVector3(const LPCSTR key, DVector3&);
	void SetVector2(const LPCSTR key, DVector2&);
	void SetColor(const LPCSTR key, DColor&);
	void SetFloat(const LPCSTR key, float);
	void SetTexture(const LPCSTR key, DTexture*);
	bool HasProperty(const LPCSTR key) const;
	void SetPass(int);
	int GetVertexUsage(int);
	int GetPassCount();
	virtual void Destroy();


private:
	DShader* m_shader;
	DShaderConstantTable* m_constantTable;
};
