#pragma once
#include <vector>

#define DLOG_MAX_SHOW_LOG_NUMS 99
#define DLOG_MAX_CACHE_LOG_NUMS 9999
#define DLOG_SHOW true

enum DLogType
{
	Info = 0,
	Warning = 1,
	Error = 2,
};

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
	static void Warn(char*);
	static void Err(char*);

private:
	void DrawMsg(DLogMsg*);

private:
	std::vector<DLogMsg*> *m_logMsgs;
};

