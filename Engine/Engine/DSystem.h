#pragma once
#include "DTime.h"
#include "DGraphics.h"
#include "DSceneManager.h"
#include "DLog.h"
#include "DInputCore.h"
#include "DRes.h"
#include "DInput.h"

/*
	系统模块管理类
*/
class DSystem
{
public:
	DSystem();
	~DSystem();
	/*初始化*/
	bool Init(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api);
	/*运行*/
	bool Run();
	/*关闭*/
	void Shutdown();
	
	static DSystem* Create(DGraphicsAPI api);

	/*获取场景管理模块*/
	static DSceneManager* GetSceneMgr();
	/*获取图形管理模块*/
	static DGraphics* GetGraphicsMgr();
	/*获取log管理模块*/
	static DLog* GetLogMgr();
	/*获取时间模块*/
	static DTime* GetTimeMgr();
	/*获取输入控制模块*/
	static IInputCore* GetInputMgr();
	/*获取资源管理模块*/
	static DRes* GetResMgr();

	static void Quit();

	//template<class T>
	static DSystem* GetInstance();

protected:
	virtual bool OnInit(int screenWidth, int screenHeight, bool fullScreen, DGraphicsAPI api) = 0;
	virtual void OnShutdown() = 0;
	virtual void OnFrameBegin() {};
	virtual void OnFrameEnd() {};
	virtual bool IsWindowShouldClose() { return false; }
	virtual IInputCore* GetInputCore() { return nullptr; }

private:
	bool Loop();

protected:
	DGraphics* m_graphicsMgr;

private:
	DTime* m_timeMgr;
	DSceneManager* m_sceneMgr;
	DLog* m_logMgr;
	DRes* m_res;
	bool m_quit;
};

static DSystem *System = 0;
