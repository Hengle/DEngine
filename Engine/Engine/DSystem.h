﻿#include "DTime.h"
#include "DGraphicsCore.h"
#include "DSceneManager.h"
#include "DLog.h"
#include "DInput.h"

#define D_DEFAULT_FULL_SCREEN    FALSE
#define D_DEFAULT_WIDTH          1024
#define D_DEFAULT_HEIGHT         768

/*
	系统类
*/
class DSystem
{
public:
	DSystem();
	~DSystem();
	bool Init();
	bool Run();
	void Shutdown();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	static HWND GetHWND();

	static DSceneManager* GetSceneMgr();
	static DGraphicsCore* GetGraphicsMgr();
	static DLog* GetLogMgr();
	static DTime* GetTimeMgr();
	static DInput* GetInputMgr();

	static void Quit();

private:
	void InitWindow(int&, int&, bool);
	bool Loop();

private:
	HWND m_hwnd;
	DTime* m_timeMgr;
	DGraphicsCore* m_graphicsMgr;
	DSceneManager* m_sceneMgr;
	DLog* m_logMgr;
	DInput* m_inputMgr;
	HINSTANCE m_hInstance;
	LPCWSTR m_applicationName;
	LPCWSTR m_title;

	bool m_quit;
};

static LRESULT CALLBACK SysWndProc(HWND, UINT, WPARAM, LPARAM);

static DSystem *System = 0;