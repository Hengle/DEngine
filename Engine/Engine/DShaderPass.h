#pragma once
#include "DGraphicsDefine.h"
#include <fstream>
#include <map>
#include <Windows.h>

/*shader��Դ����*/
typedef struct DShaderResDesc
{
public:
	DShaderResDesc()
	{
		offset = 0;
		isGlobal = false;
	}

public:
	UINT offset;//��Դƫ��
	std::string resName;//��Դ����
	bool isGlobal;//�Ƿ�Ϊȫ������
};

class DShaderProgram
{
public:
	DShaderProgram();
	/*�����������*/
	unsigned int GetPropertyCount() const;
	/*��ʼ��*/
	void Init(const char* content /*shader����*/, char* funcName /*������*/);
	/*���shader��Դ��������*/
	unsigned int GetResCount() const;
	/*Ӧ��shader����*/
	void ApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams);
	void Draw();
	bool IsInitialized();
	int GetVertexUsage();
	virtual void GetResDesc(unsigned int index, DShaderResDesc&) const = 0;
	virtual bool HasProperty(const LPCSTR key) const = 0;
	virtual void Release() = 0;

protected:
	virtual bool OnInit(const char* content, char* funcName) = 0;
	virtual void OnApplyParams(std::map<std::string, float*>&params, std::map<std::string, float*>&gparams) = 0;
	virtual void OnDraw() = 0;

protected:
	unsigned int m_cbufferCount, m_propertyCount, m_resCount;
	bool m_isInitialized;
	int m_vertexUsage;
};

class DShaderPass
{
public:
	DShaderPass();
	virtual void Release();
	void SetZWrite(char*);
	void SetZTest(char*);
	void SetCullMode(char*);
	void SetFillMode(char*);
	void SetBlendOp(char*);
	void SetBlendSrcFactor(char*);
	void SetBlendDstFactor(char*);
	void SetStencilEnable(bool);
	void SetStencilId(unsigned short);
	void SetStencilReadMask(unsigned short);
	void SetStencilWriteMask(unsigned short);
	void SetStencilComp(char*);
	void SetStencilPass(char*);
	void SetStencilFail(char*);
	void SetStencilZFail(char*);
	virtual void CompileShader(std::ifstream&) = 0;
	//DShaderRes* GetShaderRes();
	void ApplyStates();
	virtual bool HasProperty(LPCSTR key) = 0;
	virtual void Draw() = 0;
	virtual int GetVertexUsage() = 0;
	virtual int GetShaderProgramCount() = 0;
	virtual DShaderProgram* GetShaderProgram(int index) = 0;

	void SetPassEnable(bool);
	bool IsPassEnable();

private:
	DRSBlendFactor GetBlendFactor(char*);
	DRSCompareFunc GetCompFunc(char*);
	DRSStencilOp GetStencilOp(char*);

private:
	bool m_zwrite;
	bool m_enableStencil;
	DRSCompareFunc m_ztest;
	DCullMode m_cullmode;
	DFillMode m_fillmode;
	//DShaderRes* m_shaderRes;
	DRSBlendOp m_blendOp;
	DRSBlendFactor m_blendSrc;
	DRSBlendFactor m_blendDst;
	unsigned short m_stencilId;
	unsigned short m_stencilReadMask;
	unsigned short m_stencilWriteMask;
	DRSCompareFunc m_stencilComp;
	DRSStencilOp m_stencilPass;
	DRSStencilOp m_stencilFail;
	DRSStencilOp m_stencilZFail;
	bool m_enableBlend;

	bool m_passEnable;
};