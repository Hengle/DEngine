#pragma once
#include "DTime.h"
#include "DGraphics.h"
#include "DSceneManager.h"
#include "DLog.h"
#include "DInput.h"
#include "DRes.h"

#define D_DEFAULT_FULL_SCREEN    FALSE
#define D_DEFAULT_WIDTH          1024
#define D_DEFAULT_HEIGHT         768

/*
	系统模块管理类
*/
class DSystem
{
public:
	DSystem();
	~DSystem();
	/*初始化*/
	bool Init();
	/*运行*/
	bool Run();
	/*关闭*/
	void Shutdown();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	static HWND GetHWND();

	/*获取场景管理模块*/
	static DSceneManager* GetSceneMgr();
	/*获取图形管理模块*/
	static DGraphics* GetGraphicsMgr();
	/*获取log管理模块*/
	static DLog* GetLogMgr();
	/*获取时间模块*/
	static DTime* GetTimeMgr();
	/*获取输入控制模块*/
	static DInput* GetInputMgr();
	/*获取资源管理模块*/
	static DRes* GetResMgr();

	static void Quit();

private:
	void InitWindow(int&, int&, bool);
	bool Loop();

private:
	HWND m_hwnd;
	DTime* m_timeMgr;
	DGraphics* m_graphicsMgr;
	DSceneManager* m_sceneMgr;
	DLog* m_logMgr;
	DInput* m_inputMgr;
	DRes* m_res;
	HINSTANCE m_hInstance;
	LPCWSTR m_applicationName;
	LPCWSTR m_title;

	bool m_quit;
};

static LRESULT CALLBACK SysWndProc(HWND, UINT, WPARAM, LPARAM);

static DSystem *System = 0;