#include "DLog.h"
//#include "DImGUI.h"
#include "DSystem.h"
#include "atlstr.h"


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
	m_logMsgs = new std::vector<DLogMsg*>();
}

void DLog::Shutdown()
{
	int i;
	int size = m_logMsgs->size();
	for (i = 0; i < size; i++) {
		delete m_logMsgs->at(i);
	}
	m_logMsgs->clear();
	delete m_logMsgs;
	m_logMsgs = NULL;
}

void DLog::DrawMsgs()
{
	if (DLOG_SHOW == false)
		return;
	int i;
	int size = m_logMsgs->size();

	//ImGui::Begin("Console");
	
	for (i = 0; i < size && i <= DLOG_MAX_SHOW_LOG_NUMS; i++) {
		DLogMsg* msg = m_logMsgs->at(i);
		if (msg != NULL)
			DrawMsg(msg);
	}
	
	//ImGui::End();
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
	//ImVec4 color;
	
	/*if (msg->type == DLogType::Info) {
		color.x = 1;
		color.y = 1;
		color.z = 1;
		color.w = 1;
	}
	else if (msg->type == DLogType::Warning) {
		color.x = 1;
		color.y = 1;
		color.z = 0;
		color.w = 1;
	}
	else if (msg->type == DLogType::Error) {
		color.x = 1;
		color.y = 0;
		color.z = 0;
		color.w = 1;
	}*/
	//ImGui::TextColored(color, msg->msg.data());
}


void DLog::Info(char * msg)
{
	DLogMsg* lg = new DLogMsg(msg, DLogType::Info);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
}

void DLog::Warn(char * msg)
{
	DLogMsg* lg = new DLogMsg(msg, DLogType::Warning);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
}

void DLog::Err(char * msg)
{
	DLogMsg* lg = new DLogMsg(msg, DLogType::Error);
	DSystem::GetLogMgr()->AddLog(lg);

	OutputDebugString(CA2W(lg->msg.data()));
}