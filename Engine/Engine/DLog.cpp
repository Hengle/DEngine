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
	case DLogType::Info:
		GenInfoMsg(msg);
		break;
	case DLogType::Warning:
		GenWarningMsg(msg);
		break;
	case DLogType::Error:
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
	SaveLog();
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
	
	if (msg->type == DLogType::Info) {
		color.r = 1;
		color.g = 1;
		color.b = 1;
		color.a = 1;
	}
	else if (msg->type == DLogType::Warning) {
		color.r = 1;
		color.g = 1;
		color.b = 0;
		color.a = 1;
	}
	else if (msg->type == DLogType::Error) {
		color.r = 1;
		color.g = 0;
		color.b = 0;
		color.a = 1;
	}
	DGUI::Label(color, msg->msg.data());
}

void DLog::SaveLog()
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
	DLogMsg* lg = new DLogMsg(msg, DLogType::Info);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
#endif
}

void DLog::Warn(char * msg)
{
#if _DEBUG
	DLogMsg* lg = new DLogMsg(msg, DLogType::Warning);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
#endif
}

void DLog::Err(char * msg)
{
#if _DEBUG
	DLogMsg* lg = new DLogMsg(msg, DLogType::Error);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
#endif
}