#include "DLog.h"
#include "DSystem.h"
#include "atlstr.h"

void DLog::Info(char * msg)
{
	DLogMsg* lg = new DLogMsg(msg, DLogType::Info);
	DSystem::GetLogManager()->AddLog(lg);
	
	OutputDebugString(CA2W(lg->msg.data()));
}

void DLog::Warn(char * msg)
{
	DLogMsg* lg = new DLogMsg(msg, DLogType::Warning);
	DSystem::GetLogManager()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
}

void DLog::Err(char * msg)
{
	DLogMsg* lg = new DLogMsg(msg, DLogType::Error);
	DSystem::GetLogManager()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
}

DLogMsg::DLogMsg(char * msg, DLogType type)
{
	this->type = type;
	this->msg = std::string();
	switch (type)
	{
	case Info:
		GenInfoMsg(msg);
		break;
	case Warning:
		GenWarningMsg(msg);
		break;
	case Error:
		GenErrorMsg(msg);
		break;
	default:
		break;
	}
}

DLogMsg::~DLogMsg()
{
	
}

void DLogMsg::GenInfoMsg(char* msg)
{
	this->msg.append("[Info]");
	this->msg.append(msg);
	this->msg.append("\n");
}

void DLogMsg::GenWarningMsg(char* msg)
{
	this->msg.append("[Warning]");
	this->msg.append(msg);
	this->msg.append("\n");
}

void DLogMsg::GenErrorMsg(char* msg)
{
	this->msg.append("[Error]");
	this->msg.append(msg);
	this->msg.append("\n");
}
