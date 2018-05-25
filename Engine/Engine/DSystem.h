#include "DTime.h"
#include "DGraphicsCore.h"
#include "DSceneManager.h"
#include "DLogManager.h"
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

	static DSceneManager* GetSceneManager();
	static DGraphicsCore* GetGraphicsCore();
	static DLogManager* GetLogManager();
	static DTime* GetTime();

	static void Quit();

private:
	void InitWindow(int&, int&, bool);
	bool Loop();

private:
	HWND m_hwnd;
	DTime* m_time;
	DGraphicsCore* m_graphics;
	DSceneManager* m_sceneManager;
	DLogManager* m_logManager;
	DInput* m_input;
	HINSTANCE m_hInstance;
	LPCWSTR m_applicationName;
	LPCWSTR m_title;

	bool m_quit;
};

static LRESULT CALLBACK SysWndProc(HWND, UINT, WPARAM, LPARAM);

static DSystem *System = 0;