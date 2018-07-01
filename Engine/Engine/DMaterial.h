#pragma once
#include "DShader.h"
#include <map>

class DMaterial : public DResObject
{
//private:
//	struct MaterialParam
//	{
//	public:
//		MaterialParam(int length, int index, int size, int offset, int shadertype);
//		~MaterialParam();
//		void SetParam(int index, float value);
//		void Release();
//		void GetParams(int& pcount, int&pindex, int&poffset, int&psize, int&stype, float**params);
//
//	private:
//		float* m_params;
//		int m_shaderType;
//		int m_offset;
//		int m_length;
//		int m_index;
//		int m_size;
//	};

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
	void SetCullMode(DCullMode);
	void SetZWrite(bool);
	void SetZTest(DRSCompareFunc);
	bool HasProperty(const LPCSTR key) const;
	void Apply();
	int GetVertexUsage();
	virtual void Destroy();


private:
	DShader* m_shader;
	//MaterialParam** m_params;
	//std::map<std::string, float*> m_params;
	//std::map<std::string, DTexture*> m_textures;
	DShaderConstantTable* m_constantTable;
	//unsigned int m_paramCount;
	DCullMode m_cullMode;
	bool m_zwrite;
	DRSCompareFunc m_ztest;
};
