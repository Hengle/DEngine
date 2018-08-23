#include "DLog.h"
#include "DGUI.h"
#include "DSystem.h"
#include "atlstr.h"
#include <fstream>


DLogMsg::DLogMsg(char * msg, DLogType type)
{
	this->type = type;
	this->msg = std::string();
	switch (type)
	{
	case DLogType::DLogType_Info:
		GenInfoMsg(msg);
		break;
	case DLogType::DLogType_Warning:
		GenWarningMsg(msg);
		break;
	case DLogType::DLogType_Error:
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


DLog::DLog()
{
	m_logMsgs = 0;
}


DLog::~DLog()
{
}

void DLog::Init()
{
#if _DEBUG
	m_logMsgs = new std::vector<DLogMsg*>();
#endif
}

void DLog::Shutdown()
{
#if _DEBUG
	WriteLog();
	int i;
	int size = m_logMsgs->size();
	for (i = 0; i < size; i++) {
		delete m_logMsgs->at(i);
	}
	m_logMsgs->clear();
	delete m_logMsgs;
	m_logMsgs = NULL;
#endif
}

void DLog::DrawMsgs()
{
#if _DEBUG
	if (DLOG_SHOW == false)
		return;
	int i;
	int size = m_logMsgs->size();

	DGUI::BeginWindow("Console");
	
	for (i = 0; i < size && i <= DLOG_MAX_SHOW_LOG_NUMS; i++) {
		DLogMsg* msg = m_logMsgs->at(i);
		if (msg != NULL)
			DrawMsg(msg);
	}
	
	DGUI::EndWindow();
#endif
}

void DLog::AddLog(DLogMsg * msg)
{
	m_logMsgs->insert(m_logMsgs->begin(), msg);
	if (m_logMsgs->size() > DLOG_MAX_CACHE_LOG_NUMS) {
		m_logMsgs->pop_back();
	}
}

void DLog::DrawMsg(DLogMsg * msg)
{
	DColor color;
	
	if (msg->type == DLogType::DLogType_Info) {
		color.r = 1;
		color.g = 1;
		color.b = 1;
		color.a = 1;
	}
	else if (msg->type == DLogType::DLogType_Warning) {
		color.r = 1;
		color.g = 1;
		color.b = 0;
		color.a = 1;
	}
	else if (msg->type == DLogType::DLogType_Error) {
		color.r = 1;
		color.g = 0;
		color.b = 0;
		color.a = 1;
	}
	DGUI::Label(color, msg->msg.data());
}

void DLog::WriteLog()
{
#if _DEBUG
	std::ofstream fout;
	fout.open("DLog-output.txt");

	int i;
	int size = m_logMsgs->size();
	for (i = 0; i < size; i++) {
		fout << m_logMsgs->at(i)->msg.data() << endl;
	}

	fout.close();
#endif
}

void DLog::Info(char * msg)
{
#if _DEBUG
	DLogMsg* lg = new DLogMsg(msg, DLogType::DLogType_Info);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
#endif
}

void DLog::InfoArgs(char * msg, ...)
{
#if _DEBUG
	char buf[2048];
	va_list list;

	va_start(list, msg);

	vsnprintf(buf, 2048, msg, list);

	va_end(list);

	Info(buf);
#endif
}

void DLog::Warn(char * msg)
{
#if _DEBUG
	DLogMsg* lg = new DLogMsg(msg, DLogType::DLogType_Warning);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
#endif
}

void DLog::WarnArgs(char * msg, ...)
{
#if _DEBUG
	char buf[2048];
	va_list list;

	va_start(list, msg);

	vsnprintf(buf, 2048, msg, list);

	va_end(list);

	Warn(buf);
#endif
}

void DLog::Err(char * msg)
{
#if _DEBUG
	DLogMsg* lg = new DLogMsg(msg, DLogType::DLogType_Error);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
#endif
}

void DLog::ErrArgs(char * msg, ...)
{
#if _DEBUG
	char buf[2048];
	va_list list;

	va_start(list, msg);

	vsnprintf(buf, 2048, msg, list);

	va_end(list);

	Err(buf);
#endif
}

void DLog::SaveLog()
{
#if _DEBUG
	DSystem::GetLogMgr()->WriteLog();
#endif
}
