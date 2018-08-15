#pragma once
#include "DGraphicsDefine.h"
#include "DGLCore.h"
#include "DShaderPass.h"
#include <fstream>
#include <vector>

using namespace std;

//class DSubShader
//{
//public:
//	DSubShader();
//	void Release();
//	//void AddCompileTarget(DGraphicsAPI api);
//	void AddPass(DShaderPass*);
//	//bool IsSupport(DGraphicsAPI api);
//	void SetRenderQueue(DRenderQueue);
//	int GetPassCount();
//	DRenderQueue GetRenderQueue();
//	DShaderPass* GetPass(int);
//
//private:
//	//int m_compileTarget;
//	vector<DShaderPass*> m_passes;
//	DRenderQueue m_renderQueue;
//};

/*shader代码块*/
class DShaderBlock
{
private:
	struct DShaderPassLink
	{
	public:
		DShaderPass* pass = 0;
		unsigned int groupid, resid, passid;
	};

public:
	DShaderBlock();
	~DShaderBlock();
	void Release();

	/*编译*/
	bool Compile(char* fileName);
	bool IsSupported();
	int GetPassCount();
	DShaderPass* GetPass(int);
	DRenderQueue GetRenderQueue();

private:
	///*解释sub shader块*/
	//bool InterpretSubShader(ifstream&);
	/*解释ShaderBlock块*/
	bool InterpretShaderBlock(ifstream&);
	/*解释desc块*/
	void InterpretDesc(ifstream&);
	///*解释编译目标块*/
	//void InterpretCompileTarget(ifstream&, DSubShader*);
	/*解释渲染队列*/
	void InterpretRenderQueue(ifstream&);
	/*解释pass块*/
	void InterpretPass(ifstream&);
	/*解释链接pass块*/
	void InterpretLinkPass(ifstream&);
	/*解释标签块*/
	//void InterpretTags(ifstream&, DShaderPass*);
	/*解释状态块*/
	void InterpretState(ifstream&, DShaderPass*);
	/*解释模板状态块*/
	void InterpretStencil(ifstream&, DShaderPass*);
	///*解释shader块*/
	//void InterpretShader(ifstream&, DShaderPass*);
	/*解释编译目标块*/
	int InterpretCompileTarget(ifstream&);

private:
	DRenderQueue m_renderQueue;
	vector<DShaderPassLink> m_passes;
	//DSubShader* m_supportShader;
};

