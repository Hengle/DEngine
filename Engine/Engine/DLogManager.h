#pragma once
#include <vector>
#include "DLog.h"

#define DLOG_MAX_SHOW_LOG_NUMS 99
#define DLOG_MAX_CACHE_LOG_NUMS 9999
#define DLOG_SHOW true

class DLogManager
{
public:
	DLogManager();
	~DLogManager();
	void Init();
	void Shutdown();
	void DrawMsgs();
	void AddLog(DLogMsg*);

private:
	void DrawMsg(DLogMsg*);

private:
	std::vector<DLogMsg*> *m_logMsgs;
};

