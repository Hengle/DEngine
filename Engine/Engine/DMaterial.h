#pragma once
#include "DShader.h"

class DMaterial : public DResObject
{
private:
	struct MaterialParam
	{
	public:
		MaterialParam(int length, int index, int size, int offset, int shadertype);
		~MaterialParam();
		void SetParam(int index, float value);
		void Release();
		void GetParams(int& pcount, int&pindex, int&poffset, int&psize, int&stype, float**params);

	private:
		float* m_params;
		int m_shaderType;
		int m_offset;
		int m_length;
		int m_index;
		int m_size;
	};

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
	//void SetTexture(const LPCSTR cbuffername, const LPCSTR key, const DTexture&);
	bool HasProperty(const LPCSTR key) const;
	int GetParamCount() const;
	void GetParams(int index, int&pcount, int&pindex, int&poffset, int&psize, int&stype, float**params);
	virtual void Destroy();


private:
	DShader* m_shader;
	MaterialParam** m_params;
	unsigned int m_paramCount;
	//DShaderParam* m_params;
};
