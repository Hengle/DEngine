#pragma once
#include <vector>
#include <string>

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

static class DLog
{
public:
	static void Info(char*);
	static void Warn(char*);
	static void Err(char*);

};

