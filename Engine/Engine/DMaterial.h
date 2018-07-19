#pragma once
#include "DShader.h"
#include <map>

/*材质类*/
class DMaterial : public DResObject
{
public:
	DMaterial(DShader*);
	~DMaterial();
	/*获取材质的shader*/
	DShader* GetShader() const;
	void SetMatrix(const LPCSTR key, DMatrix4x4&);
	void SetVector4(const LPCSTR key, DVector4&);
	void SetVector3(const LPCSTR key, DVector3&);
	void SetVector2(const LPCSTR key, DVector2&);
	void SetColor(const LPCSTR key, DColor&);
	void SetFloat(const LPCSTR key, float);
	void SetTexture(const LPCSTR key, DTexture*);
	bool HasProperty(const LPCSTR key) const;
	void SetShader(DShader* shader);
	DShader* GetShader();
	/*设置进行渲染的shader pass，返回false则当前设置的pass不可渲染*/
	bool SetPass(int pass);
	/*判断pass是否激活*/
	bool IsPassEnable(int);
	/*设置shader pass的激活状态*/
	void SetPassEnable(int, bool);
	/*返回指定pass的顶点结构体顶点用法*/
	int GetVertexUsage(int);
	/*返回shader pass数量*/
	int GetPassCount();
	DRenderQueue GetRenderQueue();
	virtual void Destroy();


private:
	DShader* m_shader;
	/*shader常量表*/
	DShaderConstantTable* m_constantTable;
};
