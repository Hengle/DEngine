#pragma once
#include <vector>

#define DLOG_MAX_SHOW_LOG_NUMS 99
#define DLOG_MAX_CACHE_LOG_NUMS 9999
#define DLOG_SHOW true

/*log类型*/
enum DLogType
{
	/*消息*/
	DLogType_Info = 0,
	/*警告*/
	DLogType_Warning = 1,
	/*错误*/
	DLogType_Error = 2,
};

/*Log消息条目*/
typedef struct DLogMsg
{
public:
	DLogMsg(char*, DLogType);
	~DLogMsg();
	DLogType type;
	std::string msg;

private:
	void GenInfoMsg(char*);
	void GenWarningMsg(char*);
	void GenErrorMsg(char*);
} DLogMsg, *LPDLogMsg;

class DLog
{
public:
	DLog();
	~DLog();
	void Init();
	void Shutdown();
	void DrawMsgs();
	void AddLog(DLogMsg*);

	static void Info(char*);
	static void InfoArgs(char*, ...);
	static void Warn(char*);
	static void WarnArgs(char*, ...);
	static void Err(char*);
	static void ErrArgs(char*, ...);

private:
	void DrawMsg(DLogMsg*);
	void SaveLog();

private:
	std::vector<DLogMsg*> *m_logMsgs;
};

